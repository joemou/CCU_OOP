#include <iostream>
#include <map>
#include <vector>
#include <utility>
#include <cstdlib> // for rand()
#include <ctime> // for time()

using namespace std;

// Function to convert the graph to a complete graph with weights
map<pair<unsigned int, unsigned int>, int> convertToCompleteGraph(const map<unsigned int, vector<unsigned int>>& graph) {
    map<pair<unsigned int, unsigned int>, int> completeGraph;
    vector<unsigned int> vertices;
    
    // Collect all vertices
    for (const auto& node : graph) {
        vertices.push_back(node.first);
    }
    
    // Initialize random seed
    srand(time(0));
    
    // Add edges and weights
    for (size_t i = 0; i < vertices.size(); ++i) {
        for (size_t j = i + 1; j < vertices.size(); ++j) {
            unsigned int u = vertices[i];
            unsigned int v = vertices[j];
            int weight = rand() % 100 + 1; // Random weight between 1 and 100
            
            completeGraph[{u, v}] = weight;
            completeGraph[{v, u}] = weight; // Since the graph is undirected
        }
    }
    
    return completeGraph;
}

// Function to print the complete graph with weights
void printCompleteGraph(const map<pair<unsigned int, unsigned int>, int>& completeGraph) {
    for (const auto& edge : completeGraph) {
        cout << "Edge (" << edge.first.first << ", " << edge.first.second << ") has weight " << edge.second << endl;
    }
}

int main() {
    // Example tree as a map
    map<unsigned int, vector<unsigned int>> tree;
    tree[1] = {2, 3};
    tree[2] = {1, 4, 5};
    tree[3] = {1};
    tree[4] = {2};
    tree[5] = {2};

    // Convert to complete graph with weights
    map<pair<unsigned int, unsigned int>, int> completeGraph = convertToCompleteGraph(tree);

    // Print the complete graph
    printCompleteGraph(completeGraph);

    return 0;
}
