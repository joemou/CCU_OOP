#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <algorithm>

using namespace std;

// Structure to represent a graph node
typedef struct Node {

    int phy_id;
    int logicalid;
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
    graph.addEdge(1, 3);
    graph.addEdge(2, 4);
    graph.addEdge(3, 5);



    // Define start and end points by their logical IDs
    int start = 1;
    int end = 5;

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
