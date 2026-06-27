#include <bits/stdc++.h>

using namespace std;

/*
 * -----------------------------------------------------------------------------
 * TEMPLATE: Tarjan's Strongly Connected Components (SCC) Algorithm
 * -----------------------------------------------------------------------------
 * Time Complexity : O(V + E) 
 * Space Complexity: O(V + E)
 * Indexing        : 1-indexed
 * -----------------------------------------------------------------------------
 * Visual & Simplified Overview:
 *
 * A Strongly Connected Component (SCC) is a group of nodes where every node 
 * can reach every other node.
 *
 * Idea:
 * During DFS, all currently active nodes are kept in a stack.
 *
 * dfn[u] = when was u first visited?
 * low[u] = the earliest active node that u can still reach.
 *
 * Example:
 * 1 -> 2 -> 3
 * ^         |
 * |_________|
 *
 * dfn:  1=1, 2=2, 3=3
 * Since 3 can go back to 1:
 * low[3] = 1, low[2] = 1, low[1] = 1
 *
 * If (low[u] == dfn[u]), then 'u' is the root node of an SCC.
 * We keep popping nodes from the stack until we reach 'u'. 
 * All popped nodes belong to the same SCC.
 * -----------------------------------------------------------------------------
 */
struct TarjanSCC {
    int n;
    int timer;
    int scc_count;

    vector<vector<int>> adj;

    vector<int> dfn;
    vector<int> low;
    vector<int> scc_id;

    vector<bool> in_stack;

    stack<int> st;

    vector<vector<int>> sccs;

    TarjanSCC(int n)
        : n(n),
          timer(1),
          scc_count(0),
          adj(n + 1),
          dfn(n + 1, -1),
          low(n + 1, -1),
          scc_id(n + 1, -1),
          in_stack(n + 1, false) {}

    // Add a directed edge: u -> v
    void add_edge(int u, int v) {
        adj[u].push_back(v);
    }

    void dfs(int u) {
        // First visit of u
        dfn[u] = low[u] = timer++;

        // u is now active in the current DFS path
        st.push(u);
        in_stack[u] = true;

        for (int v : adj[u]) {
            if (dfn[v] == -1) {
                // First time seeing v
                dfs(v);

                // If v found an older active node, u can reach it through v
                low[u] = min(low[u], low[v]);
            }
            else if (in_stack[v]) {
                // v is still inside the DFS stack
                // u ----> v
                // This means u can reach an older active node in the current DFS path
                low[u] = min(low[u], dfn[v]);
            }
        }

        // u is the root of an SCC
        if (low[u] == dfn[u]) {
            vector<int> current_scc;

            while (true) {
                // Remove nodes belonging to the current SCC
                int v = st.top();
                st.pop();

                in_stack[v] = false;
                scc_id[v] = scc_count;

                current_scc.push_back(v);

                if (v == u)
                    break;
            }

            sccs.push_back(current_scc);
            scc_count++;
        }
    }

    // Run Tarjan on all connected parts
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
    TarjanSCC graph(vertices);

    // Adding edges using u, v format (1-indexed)
    graph.add_edge(1, 2);
    graph.add_edge(2, 3);
    graph.add_edge(3, 1); // Component 1: {1, 2, 3}

    graph.add_edge(3, 4); // Bridge edge

    graph.add_edge(4, 5);
    graph.add_edge(5, 4); // Component 2: {4, 5}

    graph.run();

    cout << "Total SCCs found: " << graph.scc_count << '\n';

    for (int i = 0; i < graph.scc_count; i++) {
        cout << "SCC " << i + 1 << ": ";
        for (int node : graph.sccs[i]) {
            cout << node << ' ';
        }
        cout << '\n';
    }

    return 0;
}
