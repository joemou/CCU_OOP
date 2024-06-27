#include <iostream>
#include <vector>
#include <queue>
#include <map>
#include <limits>
#include <utility>

using namespace std;

// Function to perform Dijkstra's algorithm
map<unsigned int, unsigned int> dijkstra(unsigned int source, const map<pair<unsigned int, unsigned int>, int> &edges, unsigned int num_nodes) {
    // Priority queue to store (distance, node) pairs
    priority_queue<pair<int, unsigned int>, vector<pair<int, unsigned int>>, greater<pair<int, unsigned int>>> pq;

    // Vector to store the minimum distance to each node
    vector<int> dist(num_nodes, numeric_limits<int>::max());
    dist[source] = 0;
    pq.push({0, source});

    // Map to store the parent of each node in the shortest path tree
    map<unsigned int, unsigned int> parent;
    parent[source] = source;

    while (!pq.empty()) {
        unsigned int u = pq.top().second;
        pq.pop();

        // Iterate over all edges outgoing from u
        for (unsigned int v = 0; v < num_nodes; ++v) {
            if (u != v && edges.count({u, v})) {
                int weight = edges.at({u, v});
                if (dist[u] + weight < dist[v]) {
                    dist[v] = dist[u] + weight;
                    pq.push({dist[v], v});
                    parent[v] = u;
                }
            }
        }
    }

    return parent;
}

int main() {
    // Define the number of nodes in the graph
    unsigned int num_nodes = 6;

    // Define the edges and their weights
    map<pair<unsigned int, unsigned int>, int> edges = {
        {{0, 1}, 4},
        {{0, 2}, 2},
        {{1, 2}, 5},
        {{1, 3}, 10},
        {{2, 4}, 3},
        {{4, 3}, 4},
        {{3, 5}, 11}
    };

    // Define the source node
    unsigned int source = 0;

    // Get the shortest path tree
    map<unsigned int, unsigned int> spt = dijkstra(source, edges, num_nodes);

    // Print the shortest path tree
    cout << "Shortest Path Tree (node: parent):" << endl;
    for (const auto &p : spt) {
        cout << p.first << ": " << p.second << endl;
    }

    return 0;
}
