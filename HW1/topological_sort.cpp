#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <stack>

using namespace std;

void dfs(int node, unordered_map<int, vector<int>>& graph, unordered_set<int>& visited, stack<int>& sorted) {
    visited.insert(node);
    for (int neighbor : graph[node]) {
        if (visited.find(neighbor) == visited.end()) {
            dfs(neighbor, graph, visited, sorted);
        }
    }
    sorted.push(node);
}

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

int main() {
    // Input precedences
    vector<pair<int, int>> precedences = {{1, 3}, {2, 1}, {3, 4}, {4, 5}, {5, 6}};

    // Constructing the graph
    unordered_map<int, vector<int>> graph;
    for (const auto& pair : precedences) {
        graph[pair.first].push_back(pair.second);
    }

    // Performing topological sort
    vector<int> sortedNodes = topologicalSort(graph);

    // Output the sorted nodes
    cout << "Sorted nodes:" << endl;
    for (int node : sortedNodes) {
        cout << node << " ";
    }
    cout << endl;

    return 0;
}
