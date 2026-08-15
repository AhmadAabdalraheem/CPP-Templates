/*
 * =============================================================================
 * TRICK 2: O(1) LCA using Euler Tour + Sparse Table
 * =============================================================================
 * COMPLEXITY:
 * - Precomputation: O(N log N)
 * - LCA Query: O(1)
 * =============================================================================
 */
struct FastLCA {
    vector<int> euler_tour;
    vector<int> depth;
    vector<int> first_appearance;
    SparseTable st; // Sparse Table configured for MIN operation on depths

    void dfs(int u, int p, int d, const vector<vector<int>>& adj) {
        first_appearance[u] = euler_tour.size();
        euler_tour.push_back(u);
        depth.push_back(d);

        for (int v : adj[u]) {
            if (v != p) {
                dfs(v, u, d + 1, adj);
                euler_tour.push_back(u);
                depth.push_back(d);
            }
        }
    }

    int get_lca(int u, int v) {
        int l = first_appearance[u];
        int r = first_appearance[v];
        if (l > r) swap(l, r);

        // O(1) Range Minimum Query returns node index with minimum depth
        return st.query_idempotent(l, r); 
    }
};
