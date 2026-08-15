// ============================================================================
// DSU (DISJOINT SET UNION) & KRUSKAL'S MST
// Usage: Connected components tracking, Cycle detection, Minimum Spanning Tree.
// Time: O(alpha(N)) per query (Almost constant O(1)) | Space: O(N)
// ============================================================================

#include <bits/stdc++.h>
using namespace std;

struct Edge {
    int u, v;
    long long w;
    bool operator<(const Edge& other) const {
        return w < other.w;
    }
};

struct DSU {
    vector<int> parent, sizes;
    vector<int> mini, maxi;
    vector<int> component_edges;
    int num_components;

    void init(int n) {
        parent.resize(n + 1);
        sizes.resize(n + 1, 1);
        mini.resize(n + 1);
        maxi.resize(n + 1);
        component_edges.resize(n + 1, 0);

        num_components = n;
        for (int i = 1; i <= n; i++) {
            parent[i] = i;
            mini[i] = i;
            maxi[i] = i;
        }
    }

    int find_root(int node) {
        if (node == parent[node]) return node;
        return parent[node] = find_root(parent[node]); // Path Compression
    }

    int get_size(int u) {
        return sizes[find_root(u)];
    }

    bool is_same_set(int u, int v) {
        return find_root(u) == find_root(v);
    }

    int count_components() {
        return num_components;
    }

    bool merge(int u, int v) {
        int root_u = find_root(u);
        int root_v = find_root(v);

        if (root_u == root_v) {
            component_edges[root_u]++; // Extra edge inside same component
            return false;
        }

        // Union by Size (Attach smaller set to larger set)
        if (sizes[root_u] > sizes[root_v]) {
            swap(root_u, root_v);
        }

        parent[root_u] = root_v;
        sizes[root_v] += sizes[root_u];
        mini[root_v] = min(mini[root_v], mini[root_u]);
        maxi[root_v] = max(maxi[root_v], maxi[root_u]);
        component_edges[root_v] += component_edges[root_u] + 1;

        num_components--;
        return true;
    }
};

// ============================================================================
// KRUSKAL'S MINIMUM SPANNING TREE (MST)
// Time: O(E log E) | Space: O(V + E)
// ============================================================================
long long kruskal(int n, vector<Edge>& edge_list) {
    sort(edge_list.begin(), edge_list.end());
    DSU dsu;
    dsu.init(n);

    long long total_weight = 0;
    int edges_cnt = 0;

    for (const auto& e : edge_list) {
        if (dsu.merge(e.u, e.v)) {
            total_weight += e.w;
            edges_cnt++;
        }
    }

    // Check if graph is connected (MST exists if edges_cnt == n - 1)
    if (edges_cnt != n - 1) return -1; // Unreachable / Disconnected

    return total_weight;
}

// Example usage: Check if all components are Complete Graphs (Clique Verification)
void solve() {
    int n, m;
    if (!(cin >> n >> m)) return;

    DSU d;
    d.init(n);

    for (int i = 0; i < m; i++) {
        int u, v;
        cin >> u >> v;
        d.merge(u, v); // Merging automatically handles edge counts correctly!
    }

    for (int i = 1; i <= n; i++) {
        if (d.find_root(i) == i) { // Check root of each component
            long long k = d.sizes[i];
            long long expected_edges = k * (k - 1) / 2;
            
            if (d.component_edges[i] != expected_edges) {
                cout << "NO\n";
                return;
            }
        }
    }

    cout << "YES\n";
}
