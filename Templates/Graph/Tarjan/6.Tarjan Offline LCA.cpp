#include <bits/stdc++.h>
using namespace std;

/*
 * =============================================================================
 * TEMPLATE: Tarjan's Offline LCA (Lowest Common Ancestor) Algorithm
 * =============================================================================
 * Time Complexity : O(V + Q * α(V)) ≈ O(V + Q)
 * Space Complexity: O(V + Q)
 * Indexing        : 1-indexed
 * =============================================================================
 *
 * WHAT THIS TEMPLATE DOES :
 * --------------------------------------------------
 * Calculates Lowest Common Ancestor (LCA) for ALL 'Q' queries simultaneously 
 * in a single DFS traversal using Disjoint Set Union (DSU).
 *
 * WHY / WHEN TO USE (متى نستخدمها بدلاً من Binary Lifting):
 * ---------------------------------------------------------
 * 1. Ultra Fast: Executes in O(V + Q) time, compared to O((V + Q) log V) for 
 *    Binary Lifting / Heavy-Light Decomposition.
 * 2. Requirements: All queries must be known offline in advance.
 * 3. Best for: Constraints where V, Q <= 10^6 and Binary Lifting causes TLE.
 *
 * HOW IT WORKS :
 * ---------------------------------
 * - Performs a single DFS over the tree.
 * - Maintains a DSU where each connected set points to its highest active ancestor.
 * - When processing node 'u' and checking query (u, v):
 *   If 'v' has already been visited, LCA(u, v) is the current DSU representative of 'v'.
 *
 * =============================================================================
 */

struct TarjanLCA {
    int n;
    int query_count;
    vector<vector<int>> adj;
    
    // queries[u] stores pairs of {v, query_id}
    vector<vector<pair<int, int>>> queries;
    
    // DSU & DFS Tracking
    vector<int> parent;
    vector<int> ancestor;
    vector<bool> visited;
    
    // Final answers for queries
    vector<int> ans;

    TarjanLCA(int n = 0) {
        init(n);
    }

    void init(int n_nodes) {
        n = n_nodes;
        query_count = 0;
        
        adj.assign(n + 1, {});
        queries.assign(n + 1, {});
        parent.assign(n + 1, 0);
        ancestor.assign(n + 1, 0);
        visited.assign(n + 1, false);
        
        for (int i = 1; i <= n; i++) {
            parent[i] = i;
            ancestor[i] = i;
        }
    }

    // Add undirected tree edge
    void add_edge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    // Add query for LCA(u, v) offline. Returns the query ID.
    int add_query(int u, int v) {
        if (u == v) {
            // Handled inline, but queued for consistent output array indexing
            queries[u].push_back({v, query_count});
        } else {
            queries[u].push_back({v, query_count});
            queries[v].push_back({u, query_count});
        }
        ans.push_back(-1); // Reserve space in answer array
        return query_count++;
    }

    // Standard DSU Find with Path Compression
    int find_set(int v) {
        if (v == parent[v]) return v;
        return parent[v] = find_set(parent[v]);
    }

    // Core Tarjan DFS
    void dfs(int u, int p = 0) {
        visited[u] = true;
        ancestor[u] = u;

        for (int v : adj[u]) {
            if (v == p) continue;
            
            dfs(v, u);
            
            // Union subtree 'v' into parent 'u'
            parent[find_set(v)] = u;
            ancestor[find_set(u)] = u;
        }

        // Answer offline queries involving node 'u'
        for (const auto& q : queries[u]) {
            int v = q.first;
            int q_id = q.second;

            if (visited[v]) {
                ans[q_id] = ancestor[find_set(v)];
            }
        }
    }

    // Runs the offline LCA solver from a given root node
    vector<int> run(int root = 1) {
        dfs(root);
        return ans;
    }

    void reset(int new_n) {
        init(new_n);
    }
};

// ============================================================================
// TEST MAIN
// ============================================================================
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    // Sample Tree Structure (1-indexed, 6 nodes):
    //         1
    //        / \
    //       2   3
    //      / \
    //     4   5
    //    /
    //   6
    int n = 6;
    TarjanLCA lca_solver(n);

    lca_solver.add_edge(1, 2);
    lca_solver.add_edge(1, 3);
    lca_solver.add_edge(2, 4);
    lca_solver.add_edge(2, 5);
    lca_solver.add_edge(4, 6);

    // Adding Offline Queries:
    int q0 = lca_solver.add_query(6, 5); // Expected LCA = 2
    int q1 = lca_solver.add_query(6, 3); // Expected LCA = 1
    int q2 = lca_solver.add_query(4, 5); // Expected LCA = 2
    int q3 = lca_solver.add_query(2, 2); // Expected LCA = 2

    // Run solver from root = 1
    vector<int> results = lca_solver.run(1);

    cout << "--- TARJAN OFFLINE LCA RESULTS ---\n";
    cout << "LCA(6, 5) = " << results[q0] << " (Expected: 2)\n";
    cout << "LCA(6, 3) = " << results[q1] << " (Expected: 1)\n";
    cout << "LCA(4, 5) = " << results[q2] << " (Expected: 2)\n";
    cout << "LCA(2, 2) = " << results[q3] << " (Expected: 2)\n";

    return 0;
}
