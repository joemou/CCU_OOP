#include <iostream>
#include <vector>
#include <unordered_map>
#include <map>
#include <set>

// Function to recursively calculate the aggregated size of each node's subtree
void calculateSubtreeSizes(unsigned int node, const std::unordered_map<unsigned int, std::vector<unsigned int>>& children, 
                           const std::vector<unsigned int>& dataSize, std::vector<unsigned int>& subtreeSize) {
    // Start with the node's own data size
    subtreeSize[node] = dataSize[node];
    
    // Check if the node has children
    if (children.find(node) != children.end()) {
        // Add the sizes of the node's children
        for (unsigned int child : children.at(node)) {
            calculateSubtreeSizes(child, children, dataSize, subtreeSize);
            subtreeSize[node] += subtreeSize[child];
        }
    }
}

int main() {
    // Example input
    std::map<unsigned int, unsigned int> tree = { {1, 0}, {2, 0}, {5, 2}, {6, 2}, {3, 1},{4, 1},{7,3} };
    std::vector<unsigned int> dataSize = {0, 1, 3, 1, 2, 1, 1, 2}; // Index 0 is dummy to align node ids with indices
    
    // Find the root node (the one with no parent)
    unsigned int root = 0;
    std::set<unsigned int> nodes;
    for (const auto& pair : tree) {
        nodes.insert(pair.first);
        nodes.insert(pair.second);
    }
    for (const auto& node : nodes) {
        if (tree.find(node) == tree.end()) {
            root = node;
            break;
        }
    }

    // Build the children list for each node
    std::unordered_map<unsigned int, std::vector<unsigned int>> children;
    for (const auto& pair : tree) {
        children[pair.second].push_back(pair.first);
    }

    // Initialize the subtree size array
    std::vector<unsigned int> subtreeSize(dataSize.size(), 0);

    // Calculate the subtree sizes starting from the root
    calculateSubtreeSizes(root, children, dataSize, subtreeSize);

    // Output the aggregated data size for each node
    for (unsigned int i = 1; i < subtreeSize.size(); ++i) {
        std::cout << "Node " << i << " aggregated data size: " << subtreeSize[i] << std::endl;
    }

    return 0;
}
