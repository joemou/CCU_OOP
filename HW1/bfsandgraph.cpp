#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <algorithm>

using namespace std;

// Structure to represent a graph node
struct Node {
    int id;
    int lid;
    string name; // Additional data in node
    vector<int> neighbors; // List of neighboring node IDs

    Node(int _id, const string& _name) : id(_id), name(_name) {}
};

class Graph {
private:
    unordered_map<int, Node*> nodes;

public:
    // Add a node to the graph
    void addNode(int id, const string& name) {
        if (nodes.find(id) == nodes.end()) {
            Node* newNode = new Node(id, name);
            nodes[id] = newNode;
        }
    }

    // Add an undirected edge between two nodes
    void addEdge(int src, int dest, const string& srcName, const string& destName) {
        addNode(src, srcName);
        addNode(dest, destName);
        // Update the neighbors list of each node
        nodes[src]->neighbors.push_back(dest);
        nodes[dest]->neighbors.push_back(src);
    }

    // Breadth First Search to find the shortest path between start and end
    vector<int> bfs(int start, int end) {
        vector<int> path;
        unordered_map<int, bool> visited;
        unordered_map<int, int> parent;
        queue<int> q;

        // Start BFS from the starting node
        q.push(start);
        visited[start] = true;

        while (!q.empty()) {
            int current = q.front();
            q.pop();

            if (current == end) {
                // Reconstruct the path if the end node is reached
                int node = end;
                while (node != start) {
                    path.push_back(node);
                    node = parent[node];
                }
                path.push_back(start);
                reverse(path.begin(), path.end());
                return path;
            }

            for (int neighbor : nodes[current]->neighbors) {
                if (!visited[neighbor]) {
                    q.push(neighbor);
                    visited[neighbor] = true;
                    parent[neighbor] = current;
                }
            }
        }

        // If end node is not reachable from start node, return an empty path
        return path;
    }

    // Print the additional data of a node
    void printNodeData(int id) {
        if (nodes.find(id) != nodes.end()) {
            cout << "Node " << id << " - Name: " << nodes[id]->name << endl;
            // You can add more data fields here if needed
        } else {
            cout << "Node with ID " << id << " not found." << endl;
        }
    }
};

int main() {
    Graph graph;

    // Add nodes and edges to the graph
    graph.addEdge(101, 102, "Node A", "Node B");
    graph.addEdge(101, 103, "Node A", "Node C");
    graph.addEdge(102, 104, "Node B", "Node D");
    graph.addEdge(103, 105, "Node C", "Node E");
    graph.addEdge(104, 105, "Node D", "Node E");
    graph.addEdge(104, 106, "Node D", "Node F");
    graph.addEdge(105, 107, "Node E", "Node G");

    // Define start and end points by their logical IDs
    int start = 103;
    int end = 107;

    // Perform BFS to find the shortest path
    vector<int> shortestPath = graph.bfs(start, end);

    // Output the shortest path
    cout << "Shortest path from " << start << " to " << end << ": ";
    for (int node : shortestPath) {
        cout << node << " ";
    }
    cout << endl;

    // Print additional data of nodes along the shortest path
    cout << "Additional data of nodes in the shortest path:" << endl;
    for (int node : shortestPath) {
        graph.printNodeData(node);
    }

    return 0;
}
