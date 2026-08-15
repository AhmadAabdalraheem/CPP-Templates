#include <bits/stdc++.h>

using namespace std;

/*
 * -----------------------------------------------------------------------------
 * TEMPLATE: Tarjan's Bridge Finding Algorithm (Multi-Edge Safe)
 * -----------------------------------------------------------------------------
 * Time Complexity : O(V + E) 
 * Space Complexity: O(V + E)
 * Indexing        : 1-indexed (supports 0-indexed if resized properly)
 * -----------------------------------------------------------------------------
 * Fixes & Features:
 * 1. Multi-Edge Safe: Uses edge_id instead of parent node 'p' to prevent false 
 *    bridges when two nodes are connected by multiple edges.
 * 2. Multi-Testcase Ready: Includes reset() to clean memory without re-instantiation.
 * -----------------------------------------------------------------------------
 */
struct TarjanBridges {
    struct Edge {
        int to;
        int id;
    };

    int n;
    int edge_count;
    int timer;

    vector<vector<Edge>> adj;
    vector<int> dfn;
    vector<int> low;
    
    // Raw original edges storing {u, v}
    vector<pair<int, int>> original_edges;
    
    // Stores the discovered bridges as pairs of {u, v}
    vector<pair<int, int>> bridges;
    vector<bool> is_bridge; // Flags bridge edges by their edge_id

    TarjanBridges(int n = 0) {
        init(n);
    }

    void init(int n_nodes) {
        n = n_nodes;
        edge_count = 0;
        timer = 1;
        
        adj.assign(n + 1, {});
        dfn.assign(n + 1, -1);
        low.assign(n + 1, -1);
        
        original_edges.clear();
        bridges.clear();
        is_bridge.clear();
    }

    // Add an undirected edge between u and v
    void add_edge(int u, int v) {
        adj[u].push_back({v, edge_count});
        adj[v].push_back({u, edge_count});
        original_edges.push_back({u, v});
        is_bridge.push_back(false);
        edge_count++;
    }

    void dfs(int u, int p_edge_id = -1) {
        dfn[u] = low[u] = timer++;

        for (const auto& edge : adj[u]) {
            int v = edge.to;
            int e_id = edge.id;

            // Skip ONLY the specific edge we came from (handles parallel edges correctly)
            if (e_id == p_edge_id) {
                continue; 
            }

            if (dfn[v] == -1) {
                // First time seeing v (Forward/Tree Edge)
                dfs(v, e_id);

                // Update low of u based on subtree of v
                low[u] = min(low[u], low[v]);

                // Bridge Condition
                if (low[v] > dfn[u]) {
                    is_bridge[e_id] = true;
                    bridges.push_back({u, v});
                }
            } 
            else {
                // v is an ancestor already visited (Back Edge)
                low[u] = min(low[u], dfn[v]);
            }
        }
    }

    // Run the bridge inspection across all connected components
    void run() {
        for (int i = 1; i <= n; i++) {
            if (dfn[i] == -1) {
                dfs(i);
            }
        }
    }

    // Reset structure for a new testcase with 'new_n' nodes
    void reset(int new_n) {
        init(new_n);
    }
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int vertices = 4;
    TarjanBridges graph(vertices);

    // Testing Multi-Edges between 1 and 2
    graph.add_edge(1, 2); // Edge ID 0
    graph.add_edge(1, 2); // Edge ID 1 (Parallel edge! Neither should be a bridge)

    // Bridge Edge
    graph.add_edge(2, 3); // Edge ID 2 (Bridge!)

    // Single edge to leaf node
    graph.add_edge(3, 4); // Edge ID 3 (Bridge!)

    graph.run();

    // Output discovered bridges
    cout << "Total Bridges found: " << graph.bridges.size() << '\n';
    for (auto edge : graph.bridges) {
        cout << "Bridge between: " << edge.first << " and " << edge.second << '\n';
    }

    return 0;
}
