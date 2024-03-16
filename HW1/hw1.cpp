#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <stack>
#include <algorithm>

using namespace std;

class logQubits {
    public:
        int id;
        int q1;
        int q2;
};

class phyQubits{
    public:
        int id;
        int q1;
        int q2;
};

//record the precednece and what else
class basic{
    public:
        int gateId;
        int logQubitID1;
        int logQubitID2;
        int precedence;
};

//the dfs for the topological sort
void dfs(int node, unordered_map<int, vector<int>>& graph, unordered_set<int>& visited, stack<int>& sorted) {
    visited.insert(node);
    for (int neighbor : graph[node]) {
        if (visited.find(neighbor) == visited.end()) {
            dfs(neighbor, graph, visited, sorted);
        }
    }
    sorted.push(node);
}
//deal with the precedence
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

signed main() {
    //cin the num
    int numLogicalQubits, numGates, numPrecedences, numPhysicalQubits, numPhysicalLinks;
    cin >> numLogicalQubits >> numGates >> numPrecedences >> numPhysicalQubits >> numPhysicalLinks;

    basic info[numGates];
    //cin gates and its logic relatoin
    for (int i = 0; i < numGates; ++i) {
        cin >> info[i].gateId >> info[i].logQubitID1 >> info[i].logQubitID2;
    }

    //cin their precedence
    unordered_map<int, vector<int>> precedence_list;
    for (int i = 0; i < numPrecedences; ++i) {
        int precedenceID, g1, g2;
        cin >> precedenceID >> g1 >> g2;
        precedence_list[g1].push_back(g2);
    }

    //sorted it and put it in to class
    vector<int> sorted_precedece = topologicalSort(precedence_list);
    int p = 0;
    for (auto it = sorted_precedece.begin(); it != sorted_precedece.end(); ++it) {
        info[*it].precedence = p;
        p++;
    }

    unordered_map<int, int> initialMapping;
    for (int i = 0; i < numLogicalQubits; ++i) {
        int logicalQubit, physicalQubit;
        cin >> logicalQubit >> physicalQubit;
        initialMapping[logicalQubit] = physicalQubit;
    }



    // Compute the swap sequence
    // Implement your algorithm here (greedy approach)

    // Output initial mapping
    cout << "Initial Mapping:" << endl;
    for (const auto& mapping : initialMapping) {
        cout << mapping.first << " " << mapping.second << endl;
    }

    // Output gate sequence including additional swaps
    cout << "Gate Sequence:" << endl;
    for (const auto& gate : gates) {
        // Implement your logic to output gates and swaps here
        cout << "CNOT q" << gate.q1 << " q" << gate.q2 << endl;
    }

    return 0;
}
