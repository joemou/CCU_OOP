#include <iostream>
#include <vector>
#include <queue>
#include <utility>
#include <algorithm>
#include <climits>

using namespace std;

typedef pair<int, int> pii;

class Graph {
    int V; // Number of vertices
    vector<vector<pii>> adj; // Adjacency list

public:
    Graph(int V);
    void addEdge(int u, int v, int w);
    void LAST(int src);
    void printLAST(const vector<int>& parent);
    int find(vector<int>& parent, int i);
    void Union(vector<int>& parent, vector<int>& rank, int x, int y);
    int kruskalMST();
};

// Graph constructor
Graph::Graph(int V) {
    this->V = V;
    adj.resize(V);
}

// Add edge to the graph
void Graph::addEdge(int u, int v, int w) {
    adj[u].push_back(make_pair(v, w));
    adj[v].push_back(make_pair(u, w));
}

// Find function for Kruskal's algorithm
int Graph::find(vector<int>& parent, int i) {
    if (parent[i] == i) {
        return i;
    }
    return parent[i] = find(parent, parent[i]);
}

// Union function for Kruskal's algorithm
void Graph::Union(vector<int>& parent, vector<int>& rank, int x, int y) {
    int xroot = find(parent, x);
    int yroot = find(parent, y);
    if (rank[xroot] < rank[yroot]) {
        parent[xroot] = yroot;
    } else if (rank[xroot] > rank[yroot]) {
        parent[yroot] = xroot;
    } else {
        parent[yroot] = xroot;
        rank[xroot]++;
    }
}

// Kruskal's algorithm to find MST weight
int Graph::kruskalMST() {
    vector<pair<int, pii>> edges;
    for (int u = 0; u < V; ++u) {
        for (auto &neighbor : adj[u]) {
            int v = neighbor.first;
            int w = neighbor.second;
            edges.push_back({w, {u, v}});
        }
    }
    sort(edges.begin(), edges.end());

    vector<int> parent(V);
    vector<int> rank(V, 0);
    for (int i = 0; i < V; ++i) {
        parent[i] = i;
    }

    int mst_weight = 0;
    for (auto &edge : edges) {
        int u = edge.second.first;
        int v = edge.second.second;
        int w = edge.first;
        int set_u = find(parent, u);
        int set_v = find(parent, v);
        if (set_u != set_v) {
            mst_weight += w;
            Union(parent, rank, set_u, set_v);
        }
    }
    return mst_weight;
}

// Print the LAST tree
void Graph::printLAST(const vector<int>& parent) {
    cout << "Edges in the 3-2 LAST:" << endl;
    for (int i = 0; i < V; ++i) {
        if (parent[i] != -1) {
            cout << parent[i] << " - " << i << endl;
        }
    }
}

// Light Approximate Shortest-path Tree (LAST) construction
void Graph::LAST(int src) {
    // Distance vector and priority queue for Dijkstra's algorithm
    vector<int> dist(V, INT_MAX);
    priority_queue<pii, vector<pii>, greater<pii>> pq;

    // Initialize distance to source node and push it to the queue
    dist[src] = 0;
    pq.push(make_pair(0, src));

    // Store parent of each node for tree construction
    vector<int> parent(V, -1);

    // Dijkstra's algorithm
    while (!pq.empty()) {
        int u = pq.top().second;
        pq.pop();

        // Process each neighbor of u
        for (auto &neighbor : adj[u]) {
            int v = neighbor.first;
            int weight = neighbor.second;

            // Relaxation step
            if (dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
                pq.push(make_pair(dist[v], v));
                parent[v] = u;
            }
        }
    }

    // Degree constraint
    vector<int> degree(V, 0);

    // Construct the LAST with constraints
    for (int v = 0; v < V; ++v) {
        if (parent[v] != -1 && degree[parent[v]] < 3 && degree[v] < 3) {
            // Add edge to the tree if degree constraints are satisfied
            degree[parent[v]]++;
            degree[v]++;
        }
    }

    // Print the LAST
    printLAST(parent);

    // Ensure the distance ratio constraint
    for (int u = 0; u < V; ++u) {
        for (int v = 0; v < V; ++v) {
            if (u != v) {
                // Compute the distance between u and v in the LAST
                int last_distance = dist[u] + dist[v];
                // Compute the shortest path distance between u and v in the original graph
                int shortest_path_distance = dist[u];
                // Check the ratio constraint
                if (last_distance > 2 * shortest_path_distance) {
                    cout << "Ratio constraint violated for nodes " << u << " and " << v << endl;
                }
            }
        }
    }

    // Compare the LAST weight to the MST weight
    int last_weight = 0;
    for (int u = 0; u < V; ++u) {
        for (auto &neighbor : adj[u]) {
            int v = neighbor.first;
            int weight = neighbor.second;
            if (parent[v] == u || parent[u] == v) {
                last_weight += weight;
            }
        }
    }
    int mst_weight = kruskalMST();
    cout << "LAST weight: " << last_weight << endl;
    cout << "MST weight: " << mst_weight << endl;
    if (last_weight > 2 * mst_weight) {
        cout << "Weight constraint violated" << endl;
    }
}

int main() {
    // Number of vertices
    int V = 9;
    Graph g(V);

    // Add edges (u, v, w) - (from, to, weight)
    g.addEdge(0, 1, 4);
    g.addEdge(0, 7, 8);
    g.addEdge(1, 2, 8);
    g.addEdge(1, 7, 11);
    g.addEdge(2, 3, 7);
    g.addEdge(2, 8, 2);
    g.addEdge(2, 5, 4);
    g.addEdge(3, 4, 9);
    g.addEdge(3, 5, 14);
    g.addEdge(4, 5, 10);
    g.addEdge(5, 6, 2);
    g.addEdge(6, 7, 1);
    g.addEdge(6, 8, 6);
    g.addEdge(7, 8, 7);

    // Source node for LAST
    int src = 0;
    g.LAST(src);

    return 0;
}
