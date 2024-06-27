#include <iostream>
#include <map>
#include <vector>
#include <queue>
#include <climits> // for INT_MAX

using namespace std;

int bfsShortestPath(const map<unsigned int, vector<unsigned int>>& graph, unsigned int start, unsigned int end) {
    // If the start or end node does not exist in the graph, return -1
    if (graph.find(start) == graph.end() || graph.find(end) == graph.end()) {
        return -1;
    }

    // Queue for BFS
    queue<unsigned int> q;
    // Map to keep track of distances from the start node
    map<unsigned int, int> distances;

    // Initialize distances to INT_MAX
    for (const auto& node : graph) {
        distances[node.first] = INT_MAX;
    }

    // Start BFS from the start node
    q.push(start);
    distances[start] = 0;

    while (!q.empty()) {
        unsigned int current = q.front();
        q.pop();

        // If we reached the end node, return the distance
        if (current == end) {
            return distances[current];
        }

        // Explore all adjacent nodes
        for (unsigned int neighbor : graph.at(current)) {
            if (distances[neighbor] == INT_MAX) { // Not visited
                q.push(neighbor);
                distances[neighbor] = distances[current] + 1;
            }
        }
    }

    // If the end node is not reachable, return -1
    return -1;
}

int main() {
    // Example unweighted graph as a map
    map<unsigned int, vector<unsigned int>> graph;
    graph[1] = {2, 3 ,6};
    graph[2] = {1, 4, 5};
    graph[3] = {1};
    graph[4] = {2};
    graph[5] = {2,6};
    graph[6] = {1,5};

    unsigned int start = 3;
    unsigned int end = 6;

    int result = bfsShortestPath(graph, start, end);

    if (result != -1) {
        cout << "The shortest path from " << start << " to " << end << " has " << result << " edges." << endl;
    } else {
        cout << "There is no path from " << start << " to " << end << "." << endl;
    }

    return 0;
}
