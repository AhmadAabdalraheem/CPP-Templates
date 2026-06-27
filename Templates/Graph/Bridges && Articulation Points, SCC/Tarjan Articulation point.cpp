#include <bits/stdc++.h>

using namespace std;

/*
 * -----------------------------------------------------------------------------
 * TEMPLATE: Tarjan's Articulation Points (Cut Vertices) Algorithm
 * -----------------------------------------------------------------------------
 * Time Complexity : O(V + E) 
 * Space Complexity: O(V + E)
 * Indexing        : 1-indexed
 * -----------------------------------------------------------------------------
 * Visual & Simplified Overview:
 *
 * An Articulation Point (Cut Vertex) is a node whose removal increases the 
 * number of connected components in an undirected graph.
 *
 * Idea:
 * During DFS on an undirected graph, we track:
 * dfn[u] = when was u first visited?
 * low[u] = the earliest node reachable from u without using its parent path.
 *
 * Conditions for a node 'u' to be an Articulation Point:
 * 1. If 'u' is the Root of the DFS tree: It must have 2 or more distinct children.
 * 2. If 'u' is NOT the Root: It must have a child 'v' such that (low[v] >= dfn[u]).
 * This means 'v' has no back-edge to reach any ancestor strictly above 'u'.
 * -----------------------------------------------------------------------------
 */
struct TarjanArticulationPoints {
    int n;
    int timer;
    vector<vector<int>> adj;
    vector<int> dfn;
    vector<int> low;
    
    // is_cut[u] will be true if node u is an articulation point
    vector<bool> is_cut;
    vector<int> cut_vertices; // Stores the final list of cut vertices

    TarjanArticulationPoints(int n)
        : n(n),
          timer(1),
          adj(n + 1),
          dfn(n + 1, -1),
          low(n + 1, -1),
          is_cut(n + 1, false) {}

    // Add an undirected edge between u and v
    void add_edge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    void dfs(int u, int p = -1) {
        dfn[u] = low[u] = timer++;
        int children = 0; // Tracks distinct DFS children to evaluate the Root node

        for (int v : adj[u]) {
            if (v == p) {
                // Skip the direct edge leading back to the parent
                continue;
            }

            if (dfn[v] == -1) {
                // First time seeing v (Forward/Tree Edge)
                dfs(v, u);
                children++;

                // Update low of u based on the subtree rooted at v
                low[u] = min(low[u], low[v]);

                // Condition for non-root nodes
                if (p != -1 && low[v] >= dfn[u]) {
                    is_cut[u] = true;
                }
            }
            else {
                // v is an ancestor already visited (Back Edge)
                low[u] = min(low[u], dfn[v]);
            }
        }

        // Condition for root node
        if (p == -1 && children > 1) {
            is_cut[u] = true;
        }
    }

    // Run the articulation points investigation and harvest results
    void run() {
        for (int i = 1; i <= n; i++) {
            if (dfn[i] == -1) {
                dfs(i);
            }
        }
        
        // Collect all marked cut vertices
        for (int i = 1; i <= n; i++) {
            if (is_cut[i]) {
                cut_vertices.push_back(i);
            }
        }
    }
};

int main() {
    // Optimize standard I/O operations for competitive programming
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int vertices = 5;
    TarjanArticulationPoints graph(vertices);

    // Building an undirected graph with articulation points
    graph.add_edge(1, 2);
    graph.add_edge(2, 3);
    graph.add_edge(3, 1); // Cycle {1, 2, 3}

    graph.add_edge(3, 4); // Node 3 is an Articulation Point

    graph.add_edge(4, 5); // Node 4 is an Articulation Point

    graph.run();

    // Output discovered articulation points
    cout << "Total Articulation Points found: " << graph.cut_vertices.size() << '\n';
    cout << "Cut Vertices: ";
    for (int node : graph.cut_vertices) {
        cout << node << " ";
    }
    cout << '\n';

    return 0;
}
