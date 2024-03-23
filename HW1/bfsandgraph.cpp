#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <algorithm>

using namespace std;

// Structure to represent a graph node
typedef struct Node {

    int phy_id;
    int logicalId;
    vector<int> neighbors; // List of neighboring node IDs

} Node;

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
            nodes[i]->logicalId = 7+i;
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
        return logpath;
    }

    // Print the additional data of a node
    void printNodeData(int id) {
        if (nodes.find(id) != nodes.end()) {
            cout << "Node " << id <<  endl;
            // You can add more data fields here if needed
        } else {
            cout << "Node with ID " << id << " not found." << endl;
        }
    }
};

int main() {
    Graph graph;

    // Add nodes and edges to the graph
    graph.addEdge(1, 2);
    graph.addEdge(2, 3);
    graph.addEdge(3, 4);
    graph.addEdge(4, 5);

    graph.setLogicalNum(5);

    

    // Define start and end points by their logical IDs
    int logical_start = 8;
    int logical_end = 12;

    // Perform BFS to find the shortest path
    vector<int> shortestPath = graph.bfs(logical_start, logical_end, 4);
    graph.swapLogicalNum(shortestPath);

    // Output the shortest path
    cout << "Shortest path from " << logical_start << " to " << logical_end << ": ";
    for (int node : shortestPath) {
        cout << node << " ";
    }
    cout << endl;

    return 0;
}
