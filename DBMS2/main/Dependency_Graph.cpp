#include <iostream>
#include <vector>
#include <string>

using namespace std;

// Function to check dependencies between transactions
vector<int> dependency(const vector<vector<string>>& transactions, const string& ops, int row, int col) {
    vector<int> dependencies; // List to store indices of dependent transactions
    for (int i = col + 1; i < transactions[0].size(); i++) {
        for (int j = 0; j < transactions.size(); j++) {
            if (!transactions[j][i].empty() && j != row &&
                ops[1] == transactions[j][i][1] &&
                ((ops[0] == 'r' && transactions[j][i][0] == 'w') ||
                 (ops[0] == 'w' && transactions[j][i][0] == 'r') ||
                 (ops[0] == 'w' && transactions[j][i][0] == 'w'))) {
                
                dependencies.push_back(j); // Add dependent transaction index to the list
            }
        }
    }
    return dependencies; // Return the list of dependencies
}


bool dfsCycleDetect(int node, const vector<vector<int>>& dependency_matrix, vector<bool>& visited, vector<bool>& recursionStack) {
    visited[node] = true;
    recursionStack[node] = true;

    for (int i = 0; i < dependency_matrix.size(); i++) {
        if (dependency_matrix[node][i] == 1) {
            if (!visited[i] && dfsCycleDetect(i, dependency_matrix, visited, recursionStack))
                return true;
            else if (recursionStack[i])
                return true;
        }
    }

    recursionStack[node] = false;
    return false;
}


void hasCycle(const vector<vector<int>>& dependency_matrix) {
    int n = dependency_matrix.size();
    vector<bool> visited(n, false);
    vector<bool> recursionStack(n, false);

    for (int i = 0; i < n; i++) {
        if (!visited[i]) {
            if (dfsCycleDetect(i, dependency_matrix, visited, recursionStack)){
                cout << "Cycle detected in the dependency graph.\n";
                return;
            }
        }
    }
    cout << "No cycle detected in the dependency graph.\n";
    return;
}

int main() {
    // // Example 1 transactions matrix Cycle Not Detected
    // vector<vector<string>> transactions = {
    //     {"ra", "", "", "", "", "", "rc", "wb", "", "", ""},
    //     {"", "", "", "ra", "wa", "", "", "", "", "", ""},
    //     {"", "ra", "rc", "", "", "", "", "", "", "wb", ""}
    // };
    // // Example 2 transactions matrix Cycle Detected
    // vector<vector<string>> transactions = {
    //     {"ra", "", "", "wa", "", ""},
    //     {"", "rb", "", "", "wb", ""},
    //     {"rc", "", "", "", "", "wc"},
    //     {"", "wa", "", "", "", ""},
    //     {"", "", "rb", "wa", "", ""} 
    // };
    // Example 3 transactions matrix Cycle Not Detected
    vector<vector<string>> transactions = {
        {"ra", "wb", "", "", "", ""},
        {"", "", "rc", "", "", "wc"},
        {"", "", "", "ra", "wb", ""},
        {"", "ra", "", "wb", "", ""}
    };

    int n = transactions.size(); // Number of transactions

    // Initialize the dependency matrix with zeros
    vector<vector<int>> dependency_matrix(n, vector<int>(n, 0));

    // Populate the dependency matrix
    for (int i = 0; i < transactions[0].size(); i++) {
        for (int j = 0; j < transactions.size(); j++) {
            if (!transactions[j][i].empty()) {
                vector<int> dependent_transactions = dependency(transactions, transactions[j][i], j, i);
                for (int dep : dependent_transactions) {
                    dependency_matrix[j][dep] = 1; // Set adjacency for dependencies
                }
            }
        }
    }

    // Print the dependency matrix
    cout << "Dependency Matrix:\n";
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cout << dependency_matrix[i][j] << " ";
        }
        cout << endl;
    }

    hasCycle(dependency_matrix);

    return 0;
}
