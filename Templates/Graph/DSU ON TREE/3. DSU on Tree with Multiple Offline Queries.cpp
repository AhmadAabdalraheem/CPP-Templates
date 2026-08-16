#include <bits/stdc++.h>
using namespace std;

/*
 * =====================================================================
 * PATTERN 4: DSU ON TREE WITH MULTIPLE OFFLINE QUERIES
 * =====================================================================
 * 
 * 📌 WHEN TO USE THIS TEMPLATE:
 * ---------------------------------------------------------------------
 * 1. You have Q queries, where each query is attached to a specific node `u`
 *    and asks a question about `u`'s subtree.
 *    Examples: 
 *    - "Query (u, k): How many nodes in u's subtree have color k?"
 *    - "Query (u, k): How many distinct colors appear at least k times in u's subtree?"
 * 2. Static tree structure where queries can be answered OFF-LINE (stored first, 
 *    answered during DFS traversal, then printed in original query order).
 * 
 * 💡 CORE IDEA & IMPLEMENTATION DETAILS:
 * ---------------------------------------------------------------------
 * - Store queries in an adjacency list of queries: `queries[u] = {{k, query_id}, ...}`.
 * - During `dfs_dsu`, after merging light children and the current node into 
 *   the global state, iterate over all offline queries for node `u`.
 * - Read the answer directly from global state in O(1) and store it in `ans_query[query_id]`.
 * 
 * ⏱️ COMPLEXITIES:
 * ---------------------------------------------------------------------
 * - Time Complexity: O(N log N + Q)
 * - Space Complexity: O(N + Q)
 * =====================================================================
 */

// =====================================================================
// 1. DYNAMIC PART (OFFLINE QUERY STRUCTURE & STATE)
// =====================================================================

const int MAXN = 200005;

struct Query {
    int target_val; // Parameter k for query
    int query_id;   // Original index to preserve order
};

int color[MAXN];
int freq[MAXN];                  // Frequency of each color in current subtree
vector<Query> node_queries[MAXN]; // Offline queries attached to node u
int ans_query[MAXN];             // Global array to store final answers for Q queries

// Add/Remove node from global frequency state
void add(int u, int p, const vector<vector<int>>& adj, int val) {
    freq[color[u]] += val;

    for (int v : adj[u]) {
        if (v != p) {
            add(v, p, adj, val);
        }
    }
}

// =====================================================================
// 2. FIXED ENGINE PART (DSU ON TREE + OFFLINE QUERY ENGINE)
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

void dfs_dsu_offline(int u, int p, const vector<vector<int>>& adj, bool keep) {
    // 1. Traverse light children without keeping state
    for (int v : adj[u]) {
        if (v != p && v != heavy[u]) {
            dfs_dsu_offline(v, u, adj, false);
        }
    }

    // 2. Traverse heavy child and keep state
    if (heavy[u] != -1) {
        dfs_dsu_offline(heavy[u], u, adj, true);
    }

    // 3. Add node u and light children to state
    freq[color[u]]++;
    for (int v : adj[u]) {
        if (v != p && v != heavy[u]) {
            add(v, u, adj, 1);
        }
    }

    // 4. ANSWER ALL OFFLINE QUERIES FOR NODE 'u'
    // State of u's subtree is fully ready in global frequency table!
    for (const auto& q : node_queries[u]) {
        // Example: Query asks "Frequency of color k in u's subtree"
        ans_query[q.query_id] = freq[q.target_val];
    }

    // 5. Clear state if 'u' is a light child
    if (!keep) {
        add(u, p, adj, -1);
    }
}

// Function to attach queries before running DFS
void add_query(int u, int target_val, int q_id) {
    node_queries[u].push_back({target_val, q_id});
}

// =====================================================================
// 3. MAIN EXECUTION EXAMPLE
// =====================================================================

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n = 5, q = 3;
    vector<vector<int>> adj(n);

    // Node colors
    color[0] = 1; color[1] = 2; color[2] = 1; color[3] = 2; color[4] = 2;

    adj[0].push_back(1); adj[1].push_back(0);
    adj[0].push_back(2); adj[2].push_back(0);
    adj[1].push_back(3); adj[3].push_back(1);
    adj[1].push_back(4); adj[4].push_back(1);

    // Read/Attach Offline Queries
    // Query 0: How many nodes in Subtree(1) have color 2?
    add_query(1, 2, 0);

    // Query 1: How many nodes in Subtree(0) have color 1?
    add_query(0, 1, 1);

    // Query 2: How many nodes in Subtree(1) have color 1?
    add_query(1, 1, 2);

    // Run DSU on tree
    dfs_sz(0, -1, adj);
    dfs_dsu_offline(0, -1, adj, true);

    // Print Offline Answers in original query order
    for (int i = 0; i < q; i++) {
        cout << "Query " << i << " Answer: " << ans_query[i] << "\n";
    }

    return 0;
}
