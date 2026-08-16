#include <bits/stdc++.h>
using namespace std;

/*
 * =====================================================================
 * PATTERN 1: KRUSKAL RECONSTRUCTION TREE (MINIMAX / BOTTLENECK PATH)
 * =====================================================================
 * 
 * 📌 WHEN TO USE THIS TEMPLATE:
 * ---------------------------------------------------------------------
 * 1. Queries asking for the maximum edge weight on the path between u and v 
 *    such that the maximum edge weight is MINIMIZED (Minimax path).
 * 2. Static graph queries that depend on Minimum Spanning Tree bottleneck edges.
 * 
 * 💡 CORE IDEA & IMPLEMENTATION DETAILS:
 * ---------------------------------------------------------------------
 * - Run Kruskal's algorithm. When adding an edge (u, v, w):
 *   - Create a new dummy node `C = ++node_cnt` with value `val[C] = w`.
 *   - Connect `C` as parent to the roots of components containing u and v.
 * - Minimum max-edge on path between u and v = `val[LCA(u, v)]` in KRT.
 * 
 * ⏱️ COMPLEXITIES:
 * ---------------------------------------------------------------------
 * - Construction: O(M log M + N log N)
 * - Bottleneck Query: O(log N) per query via LCA
 * - Space Complexity: O(N + M)
 * =====================================================================
 */

// =====================================================================
// 1. DYNAMIC PART (GRAPH STRUCTURE & PROBLEM STATE)
// =====================================================================

const int MAXN = 200005; // Max N leaves + N internal nodes = 2 * N
const int LOGN = 19;

struct Edge {
    int u, v, w;
    bool operator<(const Edge& other) const {
        return w < other.w;
    }
};

int val[MAXN]; // Stores edge weights for internal nodes (leaves have 0 or val)

// =====================================================================
// 2. FIXED ENGINE PART (KRT CONSTRUCTION + LCA ENGINE)
// =====================================================================

struct DSU {
    vector<int> parent;
    DSU(int n) {
        parent.resize(n + 1);
        iota(parent.begin(), parent.end(), 0);
    }
    int find(int i) {
        if (parent[i] == i) return i;
        return parent[i] = find(parent[i]);
    }
    bool unite(int i, int j) {
        int root_i = find(i);
        int root_j = find(j);
        if (root_i != root_j) {
            parent[root_i] = root_j;
            return true;
        }
        return false;
    }
};

vector<int> krt_adj[MAXN];
int up[MAXN][LOGN];
int depth[MAXN];
int node_cnt;

void dfs_lca(int u, int p, int d) {
    depth[u] = d;
    up[u][0] = p;
    for (int i = 1; i < LOGN; i++) {
        if (up[u][i - 1] != -1) {
            up[u][i] = up[up[u][i - 1]][i - 1];
        } else {
            up[u][i] = -1;
        }
    }

    for (int v : krt_adj[u]) {
        if (v != p) {
            dfs_lca(v, u, d + 1);
        }
    }
}

int get_lca(int u, int v) {
    if (depth[u] < depth[v]) swap(u, v);

    for (int i = LOGN - 1; i >= 0; i--) {
        if (depth[u] - (1 << i) >= depth[v]) {
            u = up[u][i];
        }
    }

    if (u == v) return u;

    for (int i = LOGN - 1; i >= 0; i--) {
        if (up[u][i] != -1 && up[u][i] != up[v][i]) {
            u = up[u][i];
            v = up[v][i];
        }
    }

    return up[u][0];
}

// Build KRT and preprocess LCA
int build_krt(int n, vector<Edge>& edges) {
    sort(edges.begin(), edges.end());
    DSU dsu(2 * n);
    node_cnt = n;

    for (int i = 1; i <= 2 * n; i++) {
        krt_adj[i].clear();
        val[i] = 0;
    }

    for (auto& e : edges) {
        int ru = dsu.find(e.u);
        int rv = dsu.find(e.v);

        if (ru != rv) {
            node_cnt++;
            val[node_cnt] = e.w;

            krt_adj[node_cnt].push_back(ru);
            krt_adj[node_cnt].push_back(rv);

            dsu.parent[ru] = node_cnt;
            dsu.parent[rv] = node_cnt;
        }
    }

    // Process forest if disconnected, otherwise root is node_cnt
    memset(up, -1, sizeof(up));
    for (int i = 1; i <= node_cnt; i++) {
        if (dsu.find(i) == i && depth[i] == 0) {
            dfs_lca(i, -1, 1);
        }
    }

    return node_cnt; // Root of KRT
}

// Query minimum max-edge weight on path between u and v
int query_bottleneck(int u, int v) {
    int lca_node = get_lca(u, v);
    return val[lca_node];
}

// =====================================================================
// 3. MAIN EXECUTION EXAMPLE
// =====================================================================

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n = 4;
    vector<Edge> edges = {
        {1, 2, 3},
        {2, 3, 5},
        {3, 4, 2},
        {1, 4, 10}
    };

    build_krt(n, edges);

    // Max edge on path between 1 and 3 in MST
    cout << "Bottleneck edge between 1 and 3: " << query_bottleneck(1, 3) << "\n"; // Ans: 5
    cout << "Bottleneck edge between 3 and 4: " << query_bottleneck(3, 4) << "\n"; // Ans: 2

    return 0;
}
