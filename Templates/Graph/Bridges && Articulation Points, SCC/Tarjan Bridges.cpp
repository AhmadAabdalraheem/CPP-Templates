#include <bits/stdc++.h>

using namespace std;

/*
 * -----------------------------------------------------------------------------
 * TEMPLATE: Tarjan's Bridge Finding Algorithm (Cut Edges)
 * -----------------------------------------------------------------------------
 * Time Complexity : O(V + E) 
 * Space Complexity: O(V + E)
 * Indexing        : 1-indexed
 * -----------------------------------------------------------------------------
 * Visual & Simplified Overview:
 *
 * A Bridge (or Cut Edge) is an edge whose removal increases the number of 
 * connected components in an undirected graph.
 *
 * Idea:
 * During DFS on an undirected graph, we track:
 * dfn[u] = when was u first visited?
 * low[u] = the earliest node reachable from u without using the edge to its parent.
 *
 * For an edge u -> v:
 * If (low[v] > dfn[u]), it means v and its descendants have absolutely NO way 
 * to reach u or any ancestor above u if we remove the edge (u, v).
 * Therefore, (u, v) is a BRIDGE.
 * -----------------------------------------------------------------------------
 */
struct TarjanBridges {
    int n;
    int timer;
    vector<vector<int>> adj;
    vector<int> dfn;
    vector<int> low;
    
    // Stores the discovered bridges as pairs of {u, v}
    vector<pair<int, int>> bridges;

    TarjanBridges(int n)
        : n(n),
          timer(1),
          adj(n + 1),
          dfn(n + 1, -1),
          low(n + 1, -1) {}

    // Add an undirected edge between u and v
    void add_edge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    void dfs(int u, int p = -1) {
        dfn[u] = low[u] = timer++;

        for (int v : adj[u]) {
            if (v == p) {
                // Skip the direct edge leading back to the parent
                continue; 
            }

            if (dfn[v] == -1) {
                // First time seeing v (Forward/Tree Edge)
                dfs(v, u);

                // Update low of u based on the subtree rooted at v
                low[u] = min(low[u], low[v]);

                // Bridge Condition
                if (low[v] > dfn[u]) {
                    // v cannot reach u or anything higher without this edge
                    bridges.push_back({u, v});
                }
            }
            else {
                // v is an ancestor already visited (Back Edge)
                low[u] = min(low[u], dfn[v]);
            }
        }
    }

    // Run the bridge inspection across all components
    void run() {
        for (int i = 1; i <= n; i++) {
            if (dfn[i] == -1) {
                dfs(i);
            }
        }
    }
};

int main() {
    // Optimize standard I/O operations for competitive programming
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int vertices = 5;
    TarjanBridges graph(vertices);

    // Building an undirected graph with a bridge
    graph.add_edge(1, 2);
    graph.add_edge(2, 3);
    graph.add_edge(3, 1); // Cycle {1, 2, 3}

    graph.add_edge(3, 4); // This edge is a BRIDGE

    graph.add_edge(4, 5); // This edge is a BRIDGE

    graph.run();

    // Output discovered bridges
    cout << "Total Bridges found: " << graph.bridges.size() << '\n';
    for (auto edge : graph.bridges) {
        cout << "Bridge between: " << edge.first << " and " << edge.second << '\n';
    }

    return 0;
}
