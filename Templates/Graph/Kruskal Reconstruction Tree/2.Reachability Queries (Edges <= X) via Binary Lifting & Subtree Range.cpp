#include <bits/stdc++.h>
using namespace std;

/*
 * =====================================================================
 * PATTERN 2: KRT REACHABILITY QUERIES (EDGES <= X)
 * =====================================================================
 * 
 * 📌 WHEN TO USE THIS TEMPLATE:
 * ---------------------------------------------------------------------
 * 1. Queries of type: "Starting at node u, which nodes are reachable using 
 *    edges of weight <= X?"
 * 2. Range queries on reachable sets (e.g., number of reachable nodes, 
 *    K-th smallest value among reachable nodes via Segment Tree over Euler Tour).
 * 
 * 💡 CORE IDEA & IMPLEMENTATION DETAILS:
 * ---------------------------------------------------------------------
 * - Use Binary Lifting from leaf `u` to jump UP to the highest ancestor `p` 
 *   such that `val[p] <= X`.
 * - All reachable nodes are the LEAVES in `p`'s subtree.
 * - Subtree of `p` maps to contiguous range `[tin[p], tout[p]]` via Euler Tour.
 * 
 * ⏱️ COMPLEXITIES:
 * ---------------------------------------------------------------------
 * - Reachable Range Jump: O(log N)
 * - Subtree Leaf Range Query: O(1) to find range [L, R]
 * =====================================================================
 */

const int MAXN = 200005;
const int LOGN = 19;

struct Edge {
    int u, v, w;
    bool operator<(const Edge& other) const {
        return w < other.w;
    }
};

int val[MAXN];
vector<int> krt_adj[MAXN];
int up[MAXN][LOGN];
int tin[MAXN], tout[MAXN], timer = 0;
int leaf_count[MAXN]; // Stores number of leaf nodes in subtree

struct DSU {
    vector<int> parent;
    DSU(int n) {
        parent.resize(n + 1);
        iota(parent.begin(), parent.end(), 0);
    }
    int find(int i) { return parent[i] == i ? i : parent[i] = find(parent[i]); }
};

void dfs_krt_reach(int u, int p, int total_leaves) {
    tin[u] = ++timer;
    up[u][0] = p;
    for (int i = 1; i < LOGN; i++) {
        if (up[u][i - 1] != -1) up[u][i] = up[up[u][i - 1]][i - 1];
        else up[u][i] = -1;
    }

    leaf_count[u] = (u <= total_leaves ? 1 : 0);

    for (int v : krt_adj[u]) {
        if (v != p) {
            dfs_krt_reach(v, u, total_leaves);
            leaf_count[u] += leaf_count[v];
        }
    }
    tout[u] = timer;
}

int build_krt_reachability(int n, vector<Edge>& edges) {
    sort(edges.begin(), edges.end());
    DSU dsu(2 * n);
    int node_cnt = n;

    for (int i = 1; i <= 2 * n; i++) krt_adj[i].clear();

    for (auto& e : edges) {
        int ru = dsu.find(e.u), rv = dsu.find(e.v);
        if (ru != rv) {
            node_cnt++;
            val[node_cnt] = e.w;
            krt_adj[node_cnt].push_back(ru);
            krt_adj[node_cnt].push_back(rv);
            dsu.parent[ru] = node_cnt;
            dsu.parent[rv] = node_cnt;
        }
    }

    memset(up, -1, sizeof(up));
    timer = 0;
    for (int i = 1; i <= node_cnt; i++) {
        if (dsu.find(i) == i) {
            dfs_krt_reach(i, -1, n);
        }
    }

    return node_cnt;
}

// Find contiguous range [L, R] in Euler Tour for all reachable nodes from u with edge <= max_w
pair<int, int> get_reachable_range(int u, int max_w) {
    // Jump up as long as ancestor edge weight <= max_w
    for (int i = LOGN - 1; i >= 0; i--) {
        if (up[u][i] != -1 && val[up[u][i]] <= max_w) {
            u = up[u][i];
        }
    }
    // u is now the maximum valid component ancestor
    return {tin[u], tout[u]};
}

// Count total reachable leaf nodes from u with edge <= max_w
int get_reachable_node_count(int u, int max_w) {
    for (int i = LOGN - 1; i >= 0; i--) {
        if (up[u][i] != -1 && val[up[u][i]] <= max_w) {
            u = up[u][i];
        }
    }
    return leaf_count[u];
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n = 5;
    vector<Edge> edges = {
        {1, 2, 2},
        {2, 3, 4},
        {3, 4, 7},
        {4, 5, 1}
    };

    build_krt_reachability(n, edges);

    // Starting from node 1, how many nodes can we reach using edges <= 3?
    cout << "Reachable nodes from 1 with edge <= 3: " << get_reachable_node_count(1, 3) << "\n"; // Nodes 1, 2 -> Ans: 2

    // Starting from node 1, how many nodes can we reach using edges <= 5?
    cout << "Reachable nodes from 1 with edge <= 5: " << get_reachable_node_count(1, 5) << "\n"; // Nodes 1, 2, 3 -> Ans: 3

    return 0;
}
