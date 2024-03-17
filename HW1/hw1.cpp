#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <stack>
#include <algorithm>
#include <queue>

using namespace std;

//record the precednece and what else
class basic{
    public:
        int gateId;
        int logQubitID1;
        int logQubitID2;
        int precedence;
};

// Custom node structure to store additional data of physical graph
class Node {
    public:
        int physical_id;       // Node ID
        int logical_id;    // Additional data (e.g., weight, label, etc.)
        Node(int p, int l) : physical_id(p), logical_id(l) {}
};

class Graph {
    public:
        Graph(int vertices) : V(vertices), graph(vertices) {}

        void addEdge(int u, int v, int lid1, int lid2) {
            graph[u].push_back(Node(v,lid1));
            graph[v].push_back(Node(u,lid2));  // Assuming an undirected graph
        }

        void BFS(int start, int end) {
            vector<bool> visited(V, false);
            vector<int> parent(V, -1);
            queue<int> queue;

            visited[start] = true;
            queue.push(start);

            while (!queue.empty()) {
                int node = queue.front();
                queue.pop();

                if (node == end) {
                    // Found the destination, reconstruct the path
                    vector<int> path;
                    cout << "Shortest path from " << start << " to " << end << ": ";
                    while (node != -1) {
                        cout << node << " ";
                        node = parent[node];
                    }
                    cout << endl;
                    return;
                }

                for (int neighbor : graph[node].logical_id) {
                    if (!visited[neighbor]) {
                        visited[neighbor] = true;
                        parent[neighbor] = node;
                        queue.push(neighbor);
                    }
                }
            }

            cout << "No path found from " << start << " to " << end << endl;
        }
    private:
        int V;  // Number of vertices
        vector<vector<Node>> graph;
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
bool compareByprecedence(const basic& a, const basic& b) {
    return a.precedence < b.precedence;
}

signed main() {
    //cin the num
    int numLogicalQubits, numGates, numPrecedences, numPhysicalQubits, numPhysicalLinks;
    cin >> numLogicalQubits >> numGates >> numPrecedences >> numPhysicalQubits >> numPhysicalLinks;

    basic info[numGates];
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
    Graph g(numPhysicalQubits);
    for (int i = 0; i < numPhysicalLinks; ++i) {
        int phyLinkID, p1, p2;
        cin >> phyLinkID >> p1 >> p2;
        g.addEdge(p1-1, p2-1);
    }

    for (auto i = info; i != info + numGates; ++i) {
        g.BFS((*i).logQubitID1, (*i).logQubitID2);
    }

    return 0;
}
