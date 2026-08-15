#include <bits/stdc++.h>
using namespace std;

// ============================================================================
// BASIC TREE DFS (SUBTREE SIZES, DEPTHS & PARENTS)
// Usage: Precomputes basic tree metrics in a single DFS pass.
// Time: O(N) | Space: O(N)
// ============================================================================
const int N = 2e5 + 5;

vector<int> adj[N];
int sz[N], depth[N], parent_node[N];

void dfs_tree(int u, int p = 0, int d = 0) {
    sz[u] = 1;
    depth[u] = d;
    parent_node[u] = p;

    for (int v : adj[u]) {
        if (v != p) {
            dfs_tree(v, u, d + 1);
            sz[u] += sz[v];
        }
    }
}

// Test Main
int main() {
    int n = 5;
    // Tree: 1-2, 1-3, 2-4, 2-5
    adj[1] = {2, 3}; adj[2] = {1, 4, 5};
    adj[3] = {1}; adj[4] = {2}; adj[5] = {2};

    dfs_tree(1);

    cout << "Subtree size of 2: " << sz[2] << " (Expected: 3)\n";
    cout << "Depth of 4: " << depth[4] << " (Expected: 2)\n";

    return 0;
}
