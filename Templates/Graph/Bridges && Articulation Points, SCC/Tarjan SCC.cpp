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
 * Brief Overview:
 * Tarjan's algorithm uses a single Depth-First Search (DFS) pass to find SCCs. 
 * It tracks two numbers for each node:
 * 1. dfn (DFS ID): The discovery timestamp of the node.
 * 2. low (Low Link): The smallest dfn reachable from this node, including 
 * through ancestors currently active in the DFS execution stack.
 *
 * Nodes are pushed onto a stack as they are visited. When a node finishes 
 * exploring its neighbors and low[u] == dfn[u], it is recognized as the root 
 * of an SCC. Nodes are then popped off the stack until 'u' is reached, 
 * grouping them into a complete SCC.
 * -----------------------------------------------------------------------------
 */
struct TarjanSCC {
    int n;
    int timer;
    int scc_count;
    vector<vector<int>> adj;
    vector<int> dfn, low, scc_id;
    vector<bool> in_stack;
    stack<int> st;
    vector<vector<int>> sccs; // Holds the vertices of each individual SCC

    // Constructor configured for 1-based indexing
    TarjanSCC(int n) : n(n), timer(1), scc_count(0), adj(n + 1), 
                       dfn(n + 1, -1), low(n + 1, -1), scc_id(n + 1, -1), in_stack(n + 1, false) {}

    // Adds a directed edge from u to v (1-indexed)
    void add_edge(int u, int v) {
        adj[u].push_back(v);
    }

    void dfs(int u) {
        dfn[u] = low[u] = timer++;
        st.push(u);
        in_stack[u] = true;

        for (int v : adj[u]) {
            if (dfn[v] == -1) { 
                // Case 1: Node 'v' has not been visited yet (Forward/Tree Edge)
                dfs(v);
                low[u] = min(low[u], low[v]);
            } else if (in_stack[v]) { 
                // Case 2: Node 'v' is an ancestor still in the current DFS path (Back Edge)
                low[u] = min(low[u], dfn[v]);
            }
        }

        // If u is the root node of an SCC, pop all its reachable descendants
        if (low[u] == dfn[u]) {
            vector<int> current_scc;
            while (true) {
                int v = st.top();
                st.pop();
                in_stack[v] = false;
                scc_id[v] = scc_count;
                current_scc.push_back(v);
                if (u == v) break;
            }
            sccs.push_back(current_scc);
            scc_count++;
        }
    }

    // Runs Tarjan's algorithm across the entire graph from 1 to n
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
    cin.tie(NULL);

    // ==========================================
    // EXAMPLE USAGE: 1-indexed Graph
    // ==========================================
    int vertices = 5; 
    TarjanSCC graph(vertices); // Vertices are numbered 1 to 5

    // Adding edges using u, v format (1-indexed)
    graph.add_edge(1, 2);
    graph.add_edge(2, 3);
    graph.add_edge(3, 1); // Component 1: {1, 2, 3}
    graph.add_edge(3, 4); // Bridge edge
    graph.add_edge(4, 5);
    graph.add_edge(5, 4); // Component 2: {4, 5}

    graph.run();

    // Printing output
    cout << "Total SCCs found: " << graph.scc_count << "\n";
    for (int i = 0; i < graph.scc_count; i++) {
        cout << "SCC " << i + 1 << ": ";
        for (int node : graph.sccs[i]) {
            cout << node << " ";
        }
        cout << "\n";
    }

    return 0;
}
