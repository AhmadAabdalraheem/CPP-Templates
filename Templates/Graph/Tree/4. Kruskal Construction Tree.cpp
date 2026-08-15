#include <bits/stdc++.h>
using namespace std;

/*
  ============================================================================
  Kruskal Reconstruction Tree (Reachability Tree / DSU Tree)
  ============================================================================

  WHEN TO USE:
  1. Reachability Queries with Constraints:
     "Which nodes are reachable from u using edges with weight <= W (or >= W)?"
  2. Bottleneck Edge in MST:
     "Maximum / minimum edge weight on the path between u and v in the MST"
     -> Equals val[LCA(u, v)] in the Kruskal Tree.
  3. Range Queries over Connected Components:
     Flattens component queries into subtree range queries via Euler Tour:
     The subtree of ancestor 'anc' corresponds to the contiguous range [tin[anc], tout[anc]].
     Combine with Persistent Segment Tree or Merge Sort Tree for K-th element, distinct counts, etc.

  HOW IT WORKS:
  - Original vertices 1..N are leaves.
  - When Kruskal merges two components via edge (u, v) with weight w:
    Create a new dummy internal node with weight w, making it the parent of both component roots.
  - Produces a tree with N leaves and at most N-1 internal nodes.
*/

struct KruskalTree {
    int n, node_cnt;
    vector<int> parent;
    vector<int> val; // Edge weights associated with internal nodes
    vector<vector<int>> adj;

    // Binary Lifting & Euler Tour
    int LOG;
    vector<vector<int>> up;
    vector<int> depth, tin, tout;
    int timer;

    KruskalTree(int n) : n(n), node_cnt(n), parent(2 * n + 1), val(2 * n + 1, 0), adj(2 * n + 1) {
        iota(parent.begin(), parent.end(), 0);
    }

    int find(int i) {
        if (parent[i] == i) return i;
        return parent[i] = find(parent[i]);
    }

    struct Edge {
        int u, v, w;
        bool operator<(const Edge& other) const {
            return w < other.w; // Min Spanning Tree (use w > other.w for Max Spanning Tree)
        }
    };

    void build(vector<Edge>& edges) {
        sort(edges.begin(), edges.end());

        for (auto& e : edges) {
            int root_u = find(e.u);
            int root_v = find(e.v);
            if (root_u != root_v) {
                node_cnt++;
                val[node_cnt] = e.w;

                // Attach roots of current components to the new internal node
                adj[node_cnt].push_back(root_u);
                adj[node_cnt].push_back(root_v);

                parent[root_u] = node_cnt;
                parent[root_v] = node_cnt;
                parent[node_cnt] = node_cnt;
            }
        }

        // Initialize Binary Lifting & Euler Tour for all roots (handles disconnected forests)
        LOG = 32 - __builtin_clz(node_cnt);
        up.assign(node_cnt + 1, vector<int>(LOG, 0));
        depth.assign(node_cnt + 1, 0);
        tin.assign(node_cnt + 1, 0);
        tout.assign(node_cnt + 1, 0);
        timer = 0;

        for (int i = node_cnt; i >= 1; --i) {
            if (find(i) == i && up[i][0] == 0) {
                dfs(i, i);
            }
        }
    }

    void dfs(int u, int p) {
        tin[u] = ++timer;
        up[u][0] = p;
        for (int i = 1; i < LOG; ++i) {
            up[u][i] = up[up[u][i - 1]][i - 1];
        }

        for (int v : adj[u]) {
            depth[v] = depth[u] + 1;
            dfs(v, u);
        }
        tout[u] = timer;
    }

    // 1. Find the highest ancestor reachable from u using edges with weight <= max_w
    int get_highest_ancestor(int u, int max_w) {
        for (int i = LOG - 1; i >= 0; --i) {
            int p = up[u][i];
            if (p != 0 && val[p] <= max_w) {
                u = p;
            }
        }
        return u;
    }

    // 2. Compute LCA of two nodes in the tree
    int get_lca(int u, int v) {
        if (depth[u] < depth[v]) swap(u, v);
        for (int i = LOG - 1; i >= 0; --i) {
            if (depth[u] - (1 << i) >= depth[v]) {
                u = up[u][i];
            }
        }
        if (u == v) return u;
        for (int i = LOG - 1; i >= 0; --i) {
            if (up[u][i] != up[v][i]) {
                u = up[u][i];
                v = up[v][i];
            }
        }
        return up[u][0];
    }

    // 3. Max edge weight on path between u and v in the MST
    int get_max_edge_on_path(int u, int v) {
        if (find(u) != find(v)) return -1; // Disconnected
        int lca = get_lca(u, v);
        return val[lca];
    }
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    /*
      Example Graph Setup:
      N = 5 vertices (1-indexed)
      Edges:
        1 - 2 (weight 3)
        2 - 3 (weight 5)
        3 - 4 (weight 2)
        4 - 5 (weight 8)
        1 - 4 (weight 10)
    */

    int n = 5;
    vector<KruskalTree::Edge> edges = {
        {1, 2, 3},
        {2, 3, 5},
        {3, 4, 2},
        {4, 5, 8},
        {1, 4, 10}
    };

    KruskalTree kt(n);
    kt.build(edges);

    // -------------------------------------------------------------------
    // Use Case 1: Bottleneck Edge (Max weight on MST path between u and v)
    // -------------------------------------------------------------------
    int max_e = kt.get_max_edge_on_path(1, 3);
    cout << "Max edge weight on MST path between 1 and 3: " << max_e << "\n";
    // Path in MST: 1 - 2 - 3 (weights 3, 5) -> Output: 5

    // -------------------------------------------------------------------
    // Use Case 2: Reachability Query (Edges <= W)
    // -------------------------------------------------------------------
    int start_node = 3;
    int max_allowed_weight = 4;

    int anc = kt.get_highest_ancestor(start_node, max_allowed_weight);
    cout << "Reachable subtree root from node " << start_node 
         << " with edge weights <= " << max_allowed_weight 
         << " is Node " << anc << "\n";

    // -------------------------------------------------------------------
    // Use Case 3: Subtree Euler Tour Range for Data Structure Integration
    // -------------------------------------------------------------------
    // The range [tin[anc], tout[anc]] in DFS order contains all original 
    // reachable leaves under ancestor 'anc'.
    cout << "DFS range for reachable component: [" 
         << kt.tin[anc] << ", " << kt.tout[anc] << "]\n";

    return 0;
}
