#include <bits/stdc++.h>
using namespace std;

// ============================================================================
// LCA VIA BINARY LIFTING
// Usage: Lowest Common Ancestor & Tree distance queries.
// Time: Precomputation O(N log N), Query O(log N) | Space: O(N log N)
// ============================================================================
const int N = 2e5 + 5;
const int LOG = 20; // 2^20 > 2e5

vector<int> adj[N];
int up[N][LOG];
int depth[N];

// DFS to initialize depths and 1st step ancestors
void dfs_lca(int u, int p = 0, int d = 0) {
    depth[u] = d;
    up[u][0] = p;

    for (int j = 1; j < LOG; j++) {
        up[u][j] = up[up[u][j - 1]][j - 1];
    }

    for (int v : adj[u]) {
        if (v != p) {
            dfs_lca(v, u, d + 1);
        }
    }
}

// Find LCA of nodes u and v in O(log N)
int get_lca(int u, int v) {
    if (depth[u] < depth[v]) swap(u, v);

    // 1. Lift 'u' to the same depth as 'v'
    for (int j = LOG - 1; j >= 0; j--) {
        if (depth[u] - (1 << j) >= depth[v]) {
            u = up[u][j];
        }
    }

    if (u == v) return u;

    // 2. Lift both 'u' and 'v' together until their parents match
    for (int j = LOG - 1; j >= 0; j--) {
        if (up[u][j] != up[v][j]) {
            u = up[u][j];
            v = up[v][j];
        }
    }

    return up[u][0];
}

// Get distance between u and v in tree
int get_dist(int u, int v) {
    return depth[u] + depth[v] - 2 * depth[get_lca(u, v)];
}

// Test Main
int main() {
    int n = 5; // Nodes 1 to 5
    // Tree structure: 1-2, 1-3, 2-4, 2-5
    adj[1].push_back(2); adj[2].push_back(1);
    adj[1].push_back(3); adj[3].push_back(1);
    adj[2].push_back(4); adj[4].push_back(2);
    adj[2].push_back(5); adj[5].push_back(2);

    dfs_lca(1, 0, 0);

    cout << "LCA(4, 5): " << get_lca(4, 5) << " (Expected: 2)\n";
    cout << "LCA(4, 3): " << get_lca(4, 3) << " (Expected: 1)\n";
    cout << "Dist(4, 3): " << get_dist(4, 3) << " (Expected: 3)\n";

    return 0;
}
