#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <limits>
#include <cmath>
#include <functional>

using namespace std;

// Define a structure to represent a node
struct Node {
    int id;
    double energyCost;
    int dataSize;
    std::vector<int> path; // Path to the sink
};

// Function to find the shortest paths from each node to the sink using Dijkstra's algorithm
std::unordered_map<int, Node> findShortestPaths(const std::vector<std::vector<std::pair<int, double>>>& graph, int sink) {
    std::unordered_map<int, Node> paths;
    std::priority_queue<std::pair<double, int>, std::vector<std::pair<double, int>>, std::greater<std::pair<double, int>>> pq;
    pq.emplace(0.0, sink);
    paths[sink] = {sink, 0.0, 0, {sink}};

    while (!pq.empty()) {
        auto current = pq.top();
        double currentCost = current.first;
        int currentNode = current.second;
        pq.pop();

        for (const auto& neighbor_cost : graph[currentNode]) {
            int neighbor = neighbor_cost.first;
            double cost = neighbor_cost.second;
            double newCost = currentCost + cost;
            if (paths.find(neighbor) == paths.end() || newCost < paths[neighbor].energyCost) {
                paths[neighbor] = {neighbor, newCost, 0, paths[currentNode].path};
                paths[neighbor].path.push_back(neighbor);
                pq.emplace(newCost, neighbor);
            }
        }
    }

    return paths;
}

// Function to build the aggregation tree using a greedy approach
void buildAggregationTree(std::unordered_map<int, Node>& paths, const std::vector<std::vector<std::pair<int, double>>>& graph, const std::unordered_map<int, int>& dataSize, int sink, int packetSize) {
    std::unordered_map<int, bool> inTree;
    inTree[sink] = true;

    for (auto& pair : paths) {
        if (pair.first != sink) {
            pair.second.dataSize = dataSize.at(pair.first);
        }
    }

    auto compare = [&paths](int a, int b) {
        return paths[a].energyCost > paths[b].energyCost;
    };
    std::priority_queue<int, std::vector<int>, decltype(compare)> pq(compare);

    pq.push(sink);

    while (!pq.empty()) {
        int currentNode = pq.top();
        pq.pop();
        inTree[currentNode] = true;

        for (const auto& neighbor_cost : graph[currentNode]) {
            int neighbor = neighbor_cost.first;
            double cost = neighbor_cost.second;
            if (!inTree[neighbor]) {
                int totalDataSize = paths[currentNode].dataSize + dataSize.at(neighbor);
                double incrementalCost = cost * std::ceil((double)totalDataSize / packetSize);
                double newCost = paths[currentNode].energyCost + incrementalCost;
                if (paths.find(neighbor) == paths.end() || newCost < paths[neighbor].energyCost) {
                    paths[neighbor].energyCost = newCost;
                    paths[neighbor].path = paths[currentNode].path;
                    paths[neighbor].path.push_back(neighbor);
                    paths[neighbor].dataSize = totalDataSize;
                }
                pq.push(neighbor);
            }
        }
    }
}

int main() {
    // Define the graph as an adjacency list where each pair is (neighbor, energy cost)
    std::vector<std::vector<std::pair<int, double>>> graph = {
        {{1, 1.0}, {2, 1.0}},  // Node 0
        {{3, 1.0}, {0, 1.0}},  // Node 1
        {{0, 1.0},{5, 1.0}, {6, 1.0}, {7, 1.0}, {8,1.0}},  // Node 2
        {{1, 1.0},{4, 1.0}, {5, 1.0}},   // Node 3
        {{3, 1.0}},
        {{2, 1.0},{3, 1.0}},
        {{2, 1.0}},
        {{2, 1.0}},  
        {{2, 1.0}},    
    };

    std::unordered_map<int, int> dataSize = {
        {0, 0}, {1, 1}, {2, 3}, {3, 3}, {4, 2}, {5, 3}, {6, 1}, {7, 2}, {8, 1}
    };

    int sink = 0;  // Sink node
    int packetSize = 9;  // Packet size

    // Find initial shortest paths from each node to the sink
    auto paths = findShortestPaths(graph, sink);

    // Build the aggregation tree using a greedy approach
    buildAggregationTree(paths, graph, dataSize, sink, packetSize);

    // Output the paths and their energy costs
    for (const auto& pair : paths) {
        int nodeId = pair.first;
        const Node& node = pair.second;
        std::cout << "Node " << nodeId << " -> Sink (Energy Cost: " << node.energyCost << ", Data Size: " << node.dataSize << "): ";
        for (size_t i = 0; i < node.path.size(); ++i) {
            std::cout << node.path[i] << (i < node.path.size() - 1 ? " -> " : "");
        }
        std::cout << std::endl;
    }

    return 0;
}
