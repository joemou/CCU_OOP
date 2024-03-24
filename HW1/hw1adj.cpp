#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <stack>
#include <algorithm>
#include <queue>
using namespace std;

// Record the precedence and other information
class Precedence {
public:
    int gateId;
    int logQubitID1;
    int logQubitID2;
    int precedence;
};

// Custom node structure to store additional data of physical graph
class Node {
public:
    int logicalId;
    int physicalId;
    vector<Node*> neighbors; // List of neighboring nodes

    Node(int id) : logicalId(id),physicalId(id) {}
};

class Graph {
private:
    vector<Node*> nodes;

public:
    Graph(int numNodes) {
        // Initialize the nodes vector with numNodes
        nodes.reserve(numNodes + 1); // Assuming node IDs start from 1
        for (int i = 1; i <= numNodes; ++i) {
            nodes.push_back(new Node(i));
        }
    }

    // Add an undirected edge between two nodes
    void addEdge(int src, int dest) {
        nodes[src]->neighbors.push_back(nodes[dest]);
        nodes[dest]->neighbors.push_back(nodes[src]);
    }

    // Get neighbors of a node
    vector<Node*>& getNeighbors(int node) {
        return nodes[node]->neighbors;
    }

    
    vector<int> bfs(int startNode, int endNode, int num) {
        vector<int> result;
        unordered_set<int> visited;
        unordered_map<int, int> parent;
        queue<int> q;

        int phyStart=-1,phyEnd=-1;

        for (int i = 0; i <= num; i++)
        {
            if (nodes[i]->logicalId == startNode) {
                phyStart = nodes[i]->physicalId; // Adjust for 1-based indexing
            }            
            if (nodes[i]->logicalId == endNode) {
                phyEnd = nodes[i]->physicalId; // Adjust for 1-based indexing
            }
        }

        q.push(phyStart);
        visited.insert(phyStart);

        while (!q.empty()) {
            int currNode = q.front();
            q.pop();
            result.push_back(currNode);

            if (currNode == phyEnd) {
                // Reconstruct the path from startNode to endNode
                int node = phyEnd;
                while (node != startNode) {
                    result.push_back(parent[node]);
                    node = parent[node];
                }
                reverse(result.begin(), result.end());
                for (auto it = result.begin(); it != result.end();it++){
                    cout << *it<<" " ;
                }
                cout << endl;
                return result;
            }

            for (Node* neighbor : nodes[currNode]->neighbors) {
                int neighborId = neighbor->physicalId;
                if (visited.find(neighborId) == visited.end()) {
                    q.push(neighborId);
                    visited.insert(neighborId);
                    parent[neighborId] = currNode;
                }
            }
        }

        // If endNode is not reachable from startNode, return an empty path
        return {};
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

// Comparison function for sorting by precedence
bool compareByPrecedence(const Precedence& a, const Precedence& b) {
    return a.precedence < b.precedence;
}

int main() {
    // Input number of logical qubits, gates, precedences, physical qubits, and physical links
    int numLogicalQubits, numGates, numPrecedences, numPhysicalQubits, numPhysicalLinks;
    cin >> numLogicalQubits >> numGates >> numPrecedences >> numPhysicalQubits >> numPhysicalLinks;

    // Input gate information
    Precedence info[numGates];
    for (int i = 0; i < numGates; ++i) {
        cin >> info[i].gateId >> info[i].logQubitID1 >> info[i].logQubitID2;
    }

    // Input precedences
    unordered_map<int, vector<int>> precedence_list;
    for (int i = 0; i < numPrecedences; ++i) {
        int precedenceID, g1, g2;
        cin >> precedenceID >> g1 >> g2;
        precedence_list[g1].push_back(g2);
    }

    // Sort precedences and assign precedence values
    vector<int> sortedPrecedence = topologicalSort(precedence_list);
    int p = 0;
    for (auto it = sortedPrecedence.begin(); it != sortedPrecedence.end(); ++it) {
        info[(*it) - 1].precedence = p;
        p++;
    }

    // Sort gates by precedence
    sort(info, info + numGates, compareByPrecedence);

    for (auto it = sortedPrecedence.begin(); it != sortedPrecedence.end(); ++it) {
        info[(*it) - 1].precedence = p;
        
    }

    // Create physical graph
    Graph g(numPhysicalQubits);
    for (int i = 0; i < numPhysicalLinks; ++i) {
        int phyLinkID, p1, p2;
        cin >> phyLinkID >> p1 >> p2;
        g.addEdge(p1-1, p2-1);
    }

    // Set logical numbers
    for (int i = 1; i <= numPhysicalQubits; i++) {
        cout << i << " " << i << endl;
    }

    // Traverse gates and perform swaps
    for (auto i = info; i != info + numGates; ++i) {
        cout << "*"<< (*i).logQubitID1<<(*i).logQubitID2<< endl;
        vector<int> shortestPath = g.bfs((*i).logQubitID1, (*i).logQubitID2, numPhysicalQubits);
        if (shortestPath.empty()) {
            continue;
        }
    }
    return 0;
}
