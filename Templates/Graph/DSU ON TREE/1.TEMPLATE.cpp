#include <bits/stdc++.h>
using namespace std;

/*
 * =====================================================================
 * PATTERN 1: STANDARD DSU ON TREE (SMALL-TO-LARGE MERGING)
 * =====================================================================
 * 
 * 📌 WHEN TO USE THIS TEMPLATE:
 * ---------------------------------------------------------------------
 * 1. Offline Subtree Queries asking for aggregated properties of subtrees,
 *    such as:
 *    - Number of distinct values/colors in subtree.
 *    - Most frequent element in subtree (and its sum or frequency).
 *    - Count of elements satisfying a condition within subtree of node u.
 * 2. Static Tree structure with no dynamic updates on edges/nodes.
 * ---------------------------------------------------------------------
 * - Time Complexity: O(N log N) with array/vector frequency table.
 * - Space Complexity: O(N).
 * =====================================================================
 */

// =====================================================================
// 1. DYNAMIC PART (CUSTOMIZE FOR YOUR PROBLEM STATE)
// =====================================================================

const int MAXN = 200005;

int color[MAXN];
int freq[MAXN];        // Global frequency table of values
int distinct_cnt = 0; // Global answer metric (e.g., number of distinct colors)
int ans[MAXN];         // Stores final answer for each node's subtree

// Add a node's data to the global state
void add(int u, int p, const vector<vector<int>>& adj, int val) {
    if (freq[color[u]] == 0 && val == 1) distinct_cnt++;
    freq[color[u]] += val;
    if (freq[color[u]] == 0 && val == -1) distinct_cnt--;

    for (int v : adj[u]) {
        if (v != p) {
            add(v, p, adj, val);
        }
    }
}

// Remove subtree data from global state (used when clearing light children)
void remove_subtree(int u, int p, const vector<vector<int>>& adj) {
    add(u, p, adj, -1);
}

// Insert subtree data into global state (used when merging light children)
void add_subtree(int u, int p, const vector<vector<int>>& adj) {
    add(u, p, adj, 1);
}


// =====================================================================
// 2. FIXED ENGINE PART (DSU ON TREE ENGINE)
// =====================================================================

int sz[MAXN];
int heavy[MAXN];

void dfs_sz(int u, int p, const vector<vector<int>>& adj) {
    sz[u] = 1;
    heavy[u] = -1;
    int max_c_size = 0;

    for (int v : adj[u]) {
        if (v == p) continue;
        dfs_sz(v, u, adj);
        sz[u] += sz[v];
        if (sz[v] > max_c_size) {
            max_c_size = sz[v];
            heavy[u] = v;
        }
    }
}

void dfs_dsu(int u, int p, const vector<vector<int>>& adj, bool keep) {
    // 1. Solve for light children without keeping state
    for (int v : adj[u]) {
        if (v != p && v != heavy[u]) {
            dfs_dsu(v, u, adj, false);
        }
    }

    // 2. Solve for heavy child and keep state
    if (heavy[u] != -1) {
        dfs_dsu(heavy[u], u, adj, true);
    }

    // 3. Merge current node and light children into heavy child's state
    if (freq[color[u]] == 0) distinct_cnt++;
    freq[color[u]]++;

    for (int v : adj[u]) {
        if (v != p && v != heavy[u]) {
            add_subtree(v, u, adj);
        }
    }

    // 4. Save answer for node 'u'
    ans[u] = distinct_cnt;

    // 5. Clear state if requested by parent (if 'u' is a light child)
    if (!keep) {
        remove_subtree(u, p, adj);
    }
}

// Helper function to initialize and run DSU on tree
void run_dsu(int n, int root, const vector<vector<int>>& adj) {
    dfs_sz(root, -1, adj);
    dfs_dsu(root, -1, adj, true);
}


// =====================================================================
// 3. MAIN EXECUTION EXAMPLE
// =====================================================================

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n = 5;
    vector<vector<int>> adj(n);

    // Node colors: Node 0=1, 1=2, 2=1, 3=2, 4=3
    color[0] = 1; color[1] = 2; color[2] = 1; color[3] = 2; color[4] = 3;

    adj[0].push_back(1); adj[1].push_back(0);
    adj[0].push_back(2); adj[2].push_back(0);
    adj[1].push_back(3); adj[3].push_back(1);
    adj[1].push_back(4); adj[4].push_back(1);

    run_dsu(n, 0, adj);

    // Print distinct color count for each node's subtree
    for (int i = 0; i < n; i++) {
        cout << "Distinct colors in Subtree(" << i << "): " << ans[i] << "\n";
    }

    return 0;
}
