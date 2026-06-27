#include <bits/stdc++.h>

using namespace std;

/*
 * -----------------------------------------------------------------------------
 * TEMPLATE: Tarjan's Biconnected Components (BCC) Algorithm
 * -----------------------------------------------------------------------------
 * Time Complexity : O(V + E) 
 * Space Complexity: O(V + E)
 * Indexing        : 1-indexed
 * -----------------------------------------------------------------------------
 * Visual & Simplified Overview:
 *
 * A Biconnected Component (BCC) is a maximal subgraph where every pair of nodes
 * is connected by at least two vertex-independent paths.
 *
 * Idea:
 * We run a standard DFS tracking dfn[u] and low[u] to find Articulation Points.
 * As we traverse, we push every explored edge (u, v) onto an edge-stack.
 *
 * When we are at node 'u' and find a child 'v' such that low[v] >= dfn[u]:
 * It means 'u' is a cut vertex, and a complete BCC has been found in the subtree 
 * of v. We pop all edges from the stack until we extract the edge (u, v).
 * -----------------------------------------------------------------------------
 */
struct TarjanBCC {
    int n;
    int timer;
    int bcc_count;
    vector<vector<int>> adj;
    vector<int> dfn;
    vector<int> low;
    
    // Stack to store edges belonging to the current exploration path
    vector<pair<int, int>> st;
    
    // bcc_edges[i] will store all edges {u, v} that form the i-th BCC
    vector<vector<pair<int, int>>> bcc_edges;

    TarjanBCC(int n)
        : n(n),
          timer(1),
          bcc_count(0),
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
            if (v == p) continue; // Skip direct link to parent

            // To avoid processing the same undirected edge twice
            if (dfn[v] < dfn[u]) {
                st.push_back({u, v}); // Push edge to stack
            }

            if (dfn[v] == -1) {
                dfs(v, u);
                low[u] = min(low[u], low[v]);

                // Articulation point/BCC condition detected
                if (low[v] >= dfn[u]) {
                    vector<pair<int, int>> current_bcc;
                    while (true) {
                        pair<int, int> edge = st.back();
                        st.pop_back();
                        current_bcc.push_back(edge);
                        
                        // Stop once we pop the edge that triggered this component
                        if (edge == make_pair(u, v) || edge == make_pair(v, u)) {
                            break;
                        }
                    }
                    bcc_edges.push_back(current_bcc);
                    bcc_count++;
                }
            }
            else {
                low[u] = min(low[u], dfn[v]);
            }
        }
    }

    // Run BCC partitioning across the graph
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

    int vertices = 6;
    TarjanBCC graph(vertices);

    // Creating a graph with 2 distinct BCCs connected by an articulation point (Node 3)
    graph.add_edge(1, 2);
    graph.add_edge(2, 3);
    graph.add_edge(3, 1); // BCC 1: Cycle {1, 2, 3}

    graph.add_edge(3, 4); 
    graph.add_edge(4, 5);
    graph.add_edge(5, 6);
    graph.add_edge(6, 3); // BCC 2: Cycle {3, 4, 5, 6}

    graph.run();

    // Printing output
    cout << "Total Biconnected Components found: " << graph.bcc_count << '\n';
    for (int i = 0; i < graph.bcc_count; i++) {
        cout << "BCC " << i + 1 << " Edges: ";
        for (auto edge : graph.bcc_edges[i]) {
            cout << "(" << edge.first << "-" << edge.second << ") ";
        }
        cout << '\n';
    }

    return 0;
}
