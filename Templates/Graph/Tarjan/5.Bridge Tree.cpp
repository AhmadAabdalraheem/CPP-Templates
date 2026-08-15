#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;


/*
 * =============================================================================
 * TEMPLATE: Bridge-Block Tree (2-Edge-Connected Components Compression)
 * =============================================================================
 * Time Complexity : O(V + E)
 * Space Complexity: O(V + E)
 * Indexing        : 1-indexed (Multi-edge safe via unique edge_id)
 * =============================================================================
 *
 * WHAT THIS CODE DOES :
 * --------------------------------------------------
 * 1. Bridge Discovery:
 *    - Identifies all Bridges (edges whose removal disconnects the graph).
 *    - Uses `edge.id` to safely handle parallel edges between the same nodes.
 *
 * 2. 2-Edge-Connected Components (2-ECC):
 *    - Groups nodes into components where EVERY pair of nodes has at least TWO 
 *      edge-disjoint paths between them (no single edge removal can separate them).
 *    - Stored in: `comp[u]` (Component ID for node u, from 1 to `comp_cnt`).
 *
 * 3. Tree Condensation:
 *    - Contracts each 2-ECC into a single super-node.
 *    - Connects components using ONLY the discovered bridge edges.
 *    - Result: A forest/tree stored in `tree[][]` where every edge is a Bridge!
 *
 * CLASSIC CP APPLICATIONS & TRICKS:
 * -------------------------------------------------------------
 * 1. Tree DP on Condensed Graph:
 *    - Once condensed into `tree`, you can run Tree DP, Diameter of Tree, or LCA.
 *    - Example: "Find maximum weight path between U and V where no bridge is crossed twice."
 *      -> Answer = Path sum in the condensed `tree` between `comp[U]` and `comp[V]`.
 *
 * 2. Minimum Edges to Make Graph 2-Edge-Connected:
 *    - Problem: "What is the minimum number of edges to add so the graph has NO bridges?"
 *    - Formula: Count the number of Leaf Nodes in the condensed `tree` (nodes with degree == 1).
 *      -> Answer = ceil(leaf_count / 2.0).
 *
 * 3. Robbins Theorem / Graph Orientation:
 *    - An undirected graph can be oriented into a Strongly Connected Directed Graph
 *      IF AND ONLY IF it has NO Bridges!
 *
 * =============================================================================
 */
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
