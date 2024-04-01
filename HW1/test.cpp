#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <stack>

using namespace std;

// Perform DFS to do topological sort
void dfs(int node, unordered_map<int, vector<int>>& graph, unordered_set<int>& visited, stack<int>& result) {
    visited.insert(node);
    for (int neighbor : graph[node]) {
        if (visited.find(neighbor) == visited.end()) {
            dfs(neighbor, graph, visited, result);
        }
    }
    result.push(node);
}

// Perform topological sort
vector<int> topologicalSort(unordered_map<int, vector<int>>& graph) {
    unordered_set<int> visited;
    stack<int> result;

    for (auto& kvp : graph) {
        int node = kvp.first;
        if (visited.find(node) == visited.end()) {
            dfs(node, graph, visited, result);
        }
    }

    vector<int> sorted;
    while (!result.empty()) {
        sorted.push_back(result.top());
        result.pop();
    }

    return sorted;
}

int main() {
    // Input data
    vector<vector<int>> precedence = {
        {1, 2}, {1, 3}, {1, 4}, {1, 5}, {2, 6},
        {3, 6}, {4, 6}, {5, 6}, {6, 7}, {6, 8},
        {7, 10}, {8, 10}, {9, 10},
    };

    // Construct the graph
    unordered_map<int, vector<int>> graph;
    for (const auto& p : precedence) {
        graph[p[0]].push_back(p[1]); // First number comes before the second number
    }

    // Perform topological sort
    vector<int> sorted = topologicalSort(graph);

    // Output the order
    cout << "Before which number, which numbers must occur first:" << endl;
    for (int node : sorted) {
        cout << node << " is before: ";
        for (int neighbor : graph[node]) {
            cout << neighbor << " ";
        }
        cout << endl;
    }

    return 0;
}
