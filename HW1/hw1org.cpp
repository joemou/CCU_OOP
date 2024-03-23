#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <stack>
#include <algorithm>
#include <queue>

using namespace std;

//record the precednece and what else
class precedence{
public:
    int gateId;
    int logQubitID1;
    int logQubitID2;
    int precedence;
};

// Custom node structure to store additional data of physical graph
typedef struct Node {
public:

    int phy_id;
    int logicalId;
    vector<int> neighbors; // List of neighboring node IDs

}Node;

class Graph {
private:
    unordered_map<int, Node*> nodes;

public:
    // Add a node to the graph
    void addNode(int phy_id) {
        if (nodes.find(phy_id) == nodes.end()) {
            Node *newNode = new Node;
            newNode->phy_id = phy_id;
            newNode->logicalId = -1;
            nodes[phy_id] = newNode;
        }
    }

    void setLogicalNum(int phy_nodes_num){
        for (int i=1; i < phy_nodes_num+1; i++){
            nodes[i]->logicalId = i;
        }
    }

    void swapLogicalNum(vector<int> logicalNum){
        vector<int>::iterator it=logicalNum.begin();
        
        while(it!=logicalNum.end()-2){
            
            //do the vector swap which is swap for the logic path
            vector<int>::iterator logicA = it;
            vector<int>::iterator logicB = it + 1;
            int temp = *it;
            *(it) = *(it + 1);
            *(it + 1) = temp;

            int phyA = -1;
            int phyB = -1;

            for (auto &pair : nodes)
            {
                if (pair.second->logicalId == *logicA){
                    phyA = pair.first;
                }
                if (pair.second->logicalId == *logicB){
                    phyB = pair.first;
                }
            }

            //by logic path, we swap the node logical value in the map
            unordered_map<int, Node*>::iterator itA = nodes.find(phyA);
            unordered_map<int, Node*>::iterator itB = nodes.find(phyB);
            cout<<"SWAP"<<" q"<< itA->second->logicalId<<" q"<< itB->second->logicalId<<endl;
            temp = itA->second->logicalId;
            itA->second->logicalId = itB->second->logicalId;
            itB->second->logicalId = temp;
            ++it;

        }
        cout<<"CNOT"<<" q"<< *(logicalNum.end()-2)<<" q"<< *(logicalNum.end()-1)<<endl;
    }
    
    // Add an undirected edge between two nodes
    void addEdge(int src, int dest)
    {
        //if there is no node add it
        addNode(src);
        addNode(dest);
        // Update the neighbors list of each node
        nodes[src]->neighbors.push_back(dest);
        nodes[dest]->neighbors.push_back(src);
    }

    // Logical breadth First Search to find the shortest path between logical start and logical end
    vector<int> bfs(int logStart, int logEnd, int num) {
        vector<int> logpath;
        unordered_map<int, bool> visited;
        unordered_map<int, int> parent;
        queue<int> q;
        
        int phyStart;

        for (int i = 1; i < num+1;i++){
            if(nodes[i]->logicalId==logStart){
                phyStart = i;
            }

        }

        // Start BFS from the starting node
        q.push(phyStart);
        visited[phyStart] = true;

        while (!q.empty()) {
            int phyCurrent = q.front();
            int logCurrent = nodes[phyCurrent]->logicalId;
            q.pop();

            if (logCurrent == logEnd) {
                // Reconstruct the path if the end node is reached
                int node = phyCurrent;
                while (node != phyStart) {
                    logpath.push_back(nodes[node]->logicalId);
                    node = parent[node];
                }
                logpath.push_back(logStart);
                reverse(logpath.begin(), logpath.end());
                return logpath;
            }

            for (int neighbor : nodes[phyCurrent]->neighbors) {
                if (!visited[neighbor]) {
                    q.push(neighbor);
                    visited[neighbor] = true;
                    parent[neighbor] = phyCurrent;
                }
            }
        }

        // If end node is not reachable from start node, return an empty path
        exit(0);
    }

};




//the dfs for the topological sort
void dfs(int node, unordered_map<int, vector<int>>& graph, unordered_set<int>& visited, stack<int>& sorted) {
    visited.insert(node);
    for (int neighbor : graph[node]) {
        if (visited.find(neighbor) == visited.end()) {
            dfs(neighbor, graph, visited, sorted);
        }
    }
    sorted.push(node);
}
//deal with the precedence
vector<int> topologicalSort(unordered_map<int, vector<int>>& graph) {
    unordered_set<int> visited;
    stack<int> sorted;

    for (auto& pair : graph) {
        int node = pair.first;
        if (visited.find(node) == visited.end()) {
            dfs(node, graph, visited, sorted);
        }
    }

    vector<int> result;
    while (!sorted.empty()) {
        result.push_back(sorted.top());
        sorted.pop();
    }
    return result;
}

//for sort precedence
bool compareByprecedence(const precedence& a, const precedence& b) {
    return a.precedence < b.precedence;
}

signed main() {
    //cin the num
    int numLogicalQubits, numGates, numPrecedences, numPhysicalQubits, numPhysicalLinks;
    cin >> numLogicalQubits >> numGates >> numPrecedences >> numPhysicalQubits >> numPhysicalLinks;

    precedence info[numGates];
    //cin gates and its logic relatoin
    for (int i = 0; i < numGates; ++i) {
        cin >> info[i].gateId >> info[i].logQubitID1 >> info[i].logQubitID2;
    }

    //cin their precedence
    unordered_map<int, vector<int>> precedence_list;
    for (int i = 0; i < numPrecedences; ++i) {
        int precedenceID, g1, g2;
        cin >> precedenceID >> g1 >> g2;
        precedence_list[g1].push_back(g2);
    }
    //sorted it and put the precedence in to logical class
    vector<int> sorted_precedece = topologicalSort(precedence_list);
    int p = 0;
    for (auto it = sorted_precedece.begin(); it != sorted_precedece.end(); ++it) {
        info[(*it)-1].precedence = p;
        p++;
    }
    //sort the logical class by the precedence
    int size = sizeof(info) / sizeof(info[0]);
    sort(info, info + size, compareByprecedence);

    //create physical graph
    Graph g;
    for (int i = 0; i < numPhysicalLinks; ++i) {
        int phyLinkID, p1, p2;
        cin >> phyLinkID >> p1 >> p2;
        g.addEdge(p1, p2);
    }

    g.setLogicalNum(numPhysicalQubits);

    for (int i = 1; i <= numPhysicalQubits;i++){
        cout << i << " " << i<<endl;
    }

    for (auto i = info; i != info + numGates; ++i)
    {
        vector<int> shortestPath = g.bfs((*i).logQubitID1, (*i).logQubitID2, numLogicalQubits);
        g.swapLogicalNum(shortestPath);
    }

    return 0;
}
