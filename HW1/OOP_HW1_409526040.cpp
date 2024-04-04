#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <stack>
#include <algorithm>
#include <deque>
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
    int size; // connective size
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
            newNode->size=0;
            nodes[phy_id] = newNode;
            
        }
    }

    

    
    // Add an undirected edge between two nodes
    void addEdge(int src, int dest)
    {
        //if there is no node add it
        addNode(src);
        addNode(dest);
        // Update the neighbors list of each node
        nodes[src]->neighbors.push_back(dest);
        nodes[src]->size++;
        nodes[dest]->neighbors.push_back(src);
        nodes[src]->size++;
    }

    

    void Maping(precedence info[], int numGates, int numPhysicalQubits) {
        vector<bool> logvisited(numGates + 1, false);
        stack<int> Gate; // Change to stack for DFS
        vector<bool> phyvisited(numPhysicalQubits + 1, false);
        unordered_map<int, int> parent;
        deque<int> q;

        // Iterate through gates in sorted precedence order
        for (int i = 0; i < numGates; ++i) {
            auto& gate = info[i];

            // Mark logical qubits as visited
            if (!logvisited[gate.logQubitID1]) {
                Gate.push(gate.logQubitID1);
                logvisited[gate.logQubitID1] = true;
            }
            if (!logvisited[gate.logQubitID2]) {
                Gate.push(gate.logQubitID2);
                logvisited[gate.logQubitID2] = true;
            }
        }

        // If there are fewer logical qubits than physical qubits, fill the remaining physical qubits
        if ((int)Gate.size() < numPhysicalQubits) {
            for (int i = 1; i <= numPhysicalQubits; ++i) {
                if (!logvisited[i]) {
                    Gate.push(i);
                    logvisited[i] = true;
                }
            }
        }

        // Start DFS from the starting node
        q.push_back(1);
        phyvisited[1] = true;

        while (!q.empty()) {
            int phyCurrent = q.back(); // Change to back for DFS
            q.pop_back();

            // Assign logical qubit to the current physical qubit
            if (!Gate.empty()) {
                nodes[phyCurrent]->logicalId = Gate.top(); // Change to top for DFS
                Gate.pop();
            }

            for (int neighbor : nodes[phyCurrent]->neighbors) {
                if (!phyvisited[neighbor]) {
                    q.push_back(neighbor);
                    phyvisited[neighbor] = true;
                }
            }
        }

        // Output the mapping result
        for (int i = 1; i <= numPhysicalQubits; ++i) {
            cout << nodes[i]->logicalId << " " << i << endl;
        }
    }


    int bidirectional_bfs(int logStart, int logEnd, int num) {
        vector<int> logpath;
        vector<int> phypath;
        unordered_map<int, bool> visitedStart, visitedEnd;
        unordered_map<int, int> parentStart, parentEnd;
        int phyStart=-1,phyEnd=-1;

        for (int i = 1; i < num+1;i++){
            if(nodes[i]->logicalId==logStart){
                phyStart = i;
            }            
            if(nodes[i]->logicalId==logEnd){
                phyEnd = i;
            }
            if(phyStart!=-1&&phyEnd!=-1){
                break;
            }
        }



        deque<int> qStart, qEnd;
        qStart.push_back(phyStart);
        qEnd.push_back(phyEnd);
        visitedStart[phyStart] = true;
        visitedEnd[phyEnd] = true;

        int meetingNode = -1;

        while (!qStart.empty() && !qEnd.empty()) {
            int phyCurrentStart = qStart.front();
            qStart.pop_front();

            for (int neighbor : nodes[phyCurrentStart]->neighbors) {
                if (!visitedStart[neighbor]) {
                    qStart.push_back(neighbor);
                    visitedStart[neighbor] = true;
                    parentStart[neighbor] = phyCurrentStart;
                    if (visitedEnd[neighbor]) {
                        meetingNode = neighbor;
                        break;
                    }
                }
            }

            if (meetingNode != -1)
                break;

            int phyCurrentEnd = qEnd.front();
            qEnd.pop_front();

            for (int neighbor : nodes[phyCurrentEnd]->neighbors) {
                if (!visitedEnd[neighbor]) {
                    qEnd.push_back(neighbor);
                    visitedEnd[neighbor] = true;
                    parentEnd[neighbor] = phyCurrentEnd;
                    if (visitedStart[neighbor]) {
                        meetingNode = neighbor;
                        break;
                    }
                }
            }

            if (meetingNode != -1)
                break;
        }

        if (meetingNode != -1) {
            // Reconstruct path from start to meeting node
           
            int node = meetingNode;
            while (node != phyStart) {
                phypath.push_back(node);
                node = parentStart[node];
            }
            phypath.push_back(phyStart);
            reverse(phypath.begin(), phypath.end());

            // Reconstruct path from end to meeting node
            node = meetingNode;
            while (node != phyEnd) {

                node = parentEnd[node];
                phypath.push_back(node);
            }


            
            if(phypath.size()>2){
                auto it = phypath.begin();
                while(it!=phypath.end()-2){
                    cout<<"SWAP"<<" q"<< nodes[*it]->logicalId<<" q"<< nodes[*(it+1)]->logicalId<<endl;
                    int temp = nodes[*it]->logicalId;
                    nodes[*it]->logicalId = nodes[*(it + 1)]->logicalId;
                    nodes[*(it + 1)]->logicalId = temp;
                    it++;
                }
            }
            
        }


        return 0;
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

    g.Maping(info,numGates,numPhysicalQubits);



    for (auto i = info; i != info + numGates; ++i)
    {
        if(g.bidirectional_bfs((*i).logQubitID1, (*i).logQubitID2, numPhysicalQubits)){
            break;
        }
        cout<<"CNOT"<<" q"<< (*i).logQubitID1<<" q"<< (*i).logQubitID2<<endl;
    }

    return 0;
}
