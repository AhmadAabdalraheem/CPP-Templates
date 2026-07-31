#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

struct BridgeTree {
    int n, m;
    struct Edge {
        int to, id;
    };

    vector<vector<Edge>> adj;
    vector<pair<int, int>> edges; // Stores raw edges (u, v)
    
    // Bridge discovery
    vector<int> tin, low;
    vector<bool> is_bridge;
    int timer;

    // Component condensation
    vector<int> comp;
    int comp_cnt;

    // Condensed Bridge Tree
    vector<vector<int>> tree;

    BridgeTree(int n) : n(n), m(0), timer(0), comp_cnt(0) {
        adj.resize(n + 1);
        tin.assign(n + 1, 0);
        low.assign(n + 1, 0);
        comp.assign(n + 1, 0);
    }

    void add_edge(int u, int v) {
        adj[u].push_back({v, m});
        adj[v].push_back({u, m});
        edges.push_back({u, v});
        m++;
    }

    // Step 1: Tarjan's algorithm to identify bridges
    void dfs_bridges(int u, int p_edge = -1) {
        tin[u] = low[u] = ++timer;
        for (const auto& edge : adj[u]) {
            int v = edge.to;
            int id = edge.id;
            if (id == p_edge) continue;

            if (tin[v]) {
                low[u] = min(low[u], tin[v]);
            } else {
                dfs_bridges(v, id);
                low[u] = min(low[u], low[v]);
                if (low[v] > tin[u]) {
                    is_bridge[id] = true;
                }
            }
        }
    }

    // Step 2: Flood-fill non-bridge edges to group 2-ECCs
    void dfs_compress(int u, int c) {
        comp[u] = c;
        for (const auto& edge : adj[u]) {
            int v = edge.to;
            int id = edge.id;
            if (comp[v] != 0 || is_bridge[id]) continue;
            dfs_compress(v, c);
        }
    }

    // Step 3: Build the condensed tree
    void build() {
        is_bridge.assign(m, false);

        // Find bridges across all connected components
        for (int i = 1; i <= n; ++i) {
            if (!tin[i]) dfs_bridges(i);
        }

        // Compress 2-edge-connected components
        for (int i = 1; i <= n; ++i) {
            if (!comp[i]) {
                comp_cnt++;
                dfs_compress(i, comp_cnt);
            }
        }

        // Add tree edges between distinct components
        tree.resize(comp_cnt + 1);
        for (int i = 0; i < m; ++i) {
            if (is_bridge[i]) {
                int u_comp = comp[edges[i].first];
                int v_comp = comp[edges[i].second];
                tree[u_comp].push_back(v_comp);
                tree[v_comp].push_back(u_comp);
            }
        }
    }
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n = 5, m = 5;
    BridgeTree bt(n);

    // Edges: (1-2), (2-3), (3-1), (1-4), (2-5)
    bt.add_edge(1, 2);
    bt.add_edge(2, 3);
    bt.add_edge(3, 1);
    bt.add_edge(4, 1);
    bt.add_edge(5, 2);

    bt.build();

    cout << "Number of 2-ECCs: " << bt.comp_cnt << "\n";
    for (int i = 1; i <= n; ++i) {
        cout << "Node " << i << " belongs to Component " << bt.comp[i] << "\n";
    }

    return 0;
}
