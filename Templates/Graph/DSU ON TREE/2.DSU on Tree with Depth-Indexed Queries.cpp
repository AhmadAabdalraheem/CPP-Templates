#include <bits/stdc++.h>
using namespace std;

/*
 * =====================================================================
 * PATTERN 2: DSU ON TREE WITH DEPTH-INDEXED QUERIES
 * =====================================================================
 * 
 * 📌 WHEN TO USE THIS TEMPLATE:
 * ---------------------------------------------------------------------
 * 1. Queries involve depth constraints in subtrees, e.g.:
 *    - "Count nodes in u's subtree at depth K".
 *    - "Find max/sum of values in u's subtree at depth depth[u] + K".
 *    - "Palindromic paths / character frequencies at depth D".
 * 
 * 💡 CORE IDEA & IMPLEMENTATION DETAILS:
 * ---------------------------------------------------------------------
 * - Maintain a global array indexed by node DEPTH (`cnt_at_depth[depth]`).
 * - Allows O(1) maintenance when adding/removing nodes by depth.
 * - Ideal for tree depth-based statistics without needing dynamic structures.
 * 
 * ⏱️ COMPLEXITIES:
 * ---------------------------------------------------------------------
 * - Time Complexity: O(N log N)
 * - Space Complexity: O(N)
 * =====================================================================
 */

const int MAXN = 200005;

int depth_node[MAXN];
int cnt_at_depth[MAXN]; // Global depth count array
int ans_depth[MAXN];     // Example: Stores count of nodes at depth = depth[u] + 1 in u's subtree

void add_depth(int u, int p, const vector<vector<int>>& adj, int val) {
    cnt_at_depth[depth_node[u]] += val;

    for (int v : adj[u]) {
        if (v != p) {
            add_depth(v, p, adj, val);
        }
    }
}

int sz[MAXN];
int heavy[MAXN];

void dfs_sz(int u, int p, int d, const vector<vector<int>>& adj) {
    sz[u] = 1;
    depth_node[u] = d;
    heavy[u] = -1;
    int max_c_size = 0;

    for (int v : adj[u]) {
        if (v == p) continue;
        dfs_sz(v, u, d + 1, adj);
        sz[u] += sz[v];
        if (sz[v] > max_c_size) {
            max_c_size = sz[v];
            heavy[u] = v;
        }
    }
}

void dfs_dsu_depth(int u, int p, const vector<vector<int>>& adj, bool keep) {
    for (int v : adj[u]) {
        if (v != p && v != heavy[u]) {
            dfs_dsu_depth(v, u, adj, false);
        }
    }

    if (heavy[u] != -1) {
        dfs_dsu_depth(heavy[u], u, adj, true);
    }

    cnt_at_depth[depth_node[u]]++;

    for (int v : adj[u]) {
        if (v != p && v != heavy[u]) {
            add_depth(v, u, adj, 1);
        }
    }

    // Example query: Count nodes at depth = depth[u] + 1 in u's subtree
    ans_depth[u] = cnt_at_depth[depth_node[u] + 1];

    if (!keep) {
        add_depth(u, p, adj, -1);
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n = 5;
    vector<vector<int>> adj(n);
    adj[0].push_back(1); adj[1].push_back(0);
    adj[0].push_back(2); adj[2].push_back(0);
    adj[1].push_back(3); adj[3].push_back(1);
    adj[1].push_back(4); adj[4].push_back(1);

    dfs_sz(0, -1, 0, adj);
    dfs_dsu_depth(0, -1, adj, true);

    for (int i = 0; i < n; i++) {
        cout << "Nodes at depth (" << depth_node[i] + 1 << ") in Subtree(" << i << "): " << ans_depth[i] << "\n";
    }

    return 0;
}
