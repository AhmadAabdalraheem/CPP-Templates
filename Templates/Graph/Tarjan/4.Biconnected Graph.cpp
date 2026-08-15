#include <bits/stdc++.h>

using namespace std;

/*
 * Key Features:
 * 1. Finds Biconnected Components (BCCs) as sets of nodes/edges.
 * 2. Identifies all Articulation Points (Cut Vertices).
 * 3. Builds the Block-Cut Tree (BCT): A bipartite tree connecting Cut Vertices 
 *    (IDs: 1..n) with Block Nodes (IDs: n + 1 .. n + bcc_count).
 * -----------------------------------------------------------------------------
 */
 /*
 * =============================================================================
 * TEMPLATE: Tarjan's BCC, Articulation Points & Block-Cut Tree (BCT)
 * =============================================================================
 * Time Complexity : O(V + E)
 * Space Complexity: O(V + E)
 * Indexing        : 1-indexed (Handles isolated nodes & parallel edges safely)
 * =============================================================================
 *
 * WHAT THIS TEMPLATE DOES:
 * --------------------------------------------------
 * 1. Articulation Points (Cut Vertices):
 *    - Finds all critical nodes whose removal increases the number of connected 
 *      components.
 *    - Stored in: `cut_vertices` and flagged in `is_cut_vertex[u]`.
 *
 * 2. Biconnected Components (BCCs / Blocks):
 *    - Decomposes the graph into maximal 2-vertex-connected subgraphs (blocks).
 *    - Inside any BCC, no single vertex removal can disconnect it.
 *    - Stored in: `bcc_nodes[i]` (nodes in block i) and `bcc_edges[i]` (edges).
 *
 * 3. Block-Cut Tree (BCT):
 *    - Contracts the entire graph with cycles into a Bipartite Tree structure.
 *    - BCT Node Mapping:
 *        * IDs 1 .. N                     : Original Cut Vertices.
 *        * IDs (N + 1) .. (N + bcc_count) : Virtual Block Nodes representing BCCs.
 *    - Connects each Cut Vertex directly to the Block Nodes it belongs to.
 *
 * WHEN TO USE IN COMPETITIVE PROGRAMMING:
 * -------------------------------------------------------
 * 1. Graph-to-Tree Reduction:
 *    - When asked path/reachability queries under vertex removals:
 *      "If node U is removed, can we go from A to B?"
 *      -> Convert graph to BCT, then check if U lies on the simple path between
 *         A and B using Tree LCA in O(log N).
 *
 * 2. Critical Node & Component Counting:
 *    - Problems asking to find bottleneck vertices or minimum vertices to remove
 *      to separate specific sets of nodes.
 *
 * 3. Safe Against Graph Edge Cases:
 *    - Handles parallel edges correctly via unique `edge_id`.
 *    - Handles isolated degree-0 nodes correctly.
 *
 * =============================================================================
 */
struct TarjanBCC {
    struct Edge {
        int to;
        int id;
    };

    int n;
    int edge_count;
    int timer;
    int bcc_count;

    vector<vector<Edge>> adj;
    vector<int> dfn;
    vector<int> low;
    
    vector<bool> is_cut_vertex;
    vector<int> cut_vertices;

    // Stores edges of each BCC
    vector<vector<pair<int, int>>> bcc_edges;
    // Stores unique nodes belonging to each BCC
    vector<vector<int>> bcc_nodes;
    
    // Edge stack for BCC extraction
    vector<pair<int, int>> st;

    // Block-Cut Tree (BCT)
    // Nodes 1..n are original graph nodes.
    // Nodes (n + 1) .. (n + bcc_count) represent the Blocks (BCCs).
    vector<vector<int>> bct;

    TarjanBCC(int n = 0) {
        init(n);
    }

    void init(int n_nodes) {
        n = n_nodes;
        edge_count = 0;
        timer = 1;
        bcc_count = 0;

        adj.assign(n + 1, {});
        dfn.assign(n + 1, -1);
        low.assign(n + 1, -1);
        is_cut_vertex.assign(n + 1, false);

        cut_vertices.clear();
        bcc_edges.clear();
        bcc_nodes.clear();
        st.clear();
        bct.clear();
    }

    void add_edge(int u, int v) {
        adj[u].push_back({v, edge_count});
        adj[v].push_back({u, edge_count});
        edge_count++;
    }

    void dfs(int u, int p_edge_id = -1) {
        dfn[u] = low[u] = timer++;
        int children_count = 0;

        for (const auto& edge : adj[u]) {
            int v = edge.to;
            int e_id = edge.id;

            if (e_id == p_edge_id) continue; // Skip edge to parent

            if (dfn[v] < dfn[u]) {
                st.push_back({u, v}); // Push active edge
            }

            if (dfn[v] == -1) {
                children_count++;
                dfs(v, e_id);

                low[u] = min(low[u], low[v]);

                // Condition for Articulation Point & BCC creation
                if (low[v] >= dfn[u]) {
                    if (p_edge_id != -1) {
                        is_cut_vertex[u] = true;
                    }

                    vector<pair<int, int>> current_bcc_edges;
                    vector<int> current_bcc_nodes;

                    while (true) {
                        pair<int, int> edge_top = st.back();
                        st.pop_back();

                        current_bcc_edges.push_back(edge_top);
                        current_bcc_nodes.push_back(edge_top.first);
                        current_bcc_nodes.push_back(edge_top.second);

                        if (edge_top == make_pair(u, v) || edge_top == make_pair(v, u)) {
                            break;
                        }
                    }

                    // Remove duplicate node IDs in the BCC
                    sort(current_bcc_nodes.begin(), current_bcc_nodes.end());
                    current_bcc_nodes.erase(unique(current_bcc_nodes.begin(), current_bcc_nodes.end()), current_bcc_nodes.end());

                    bcc_edges.push_back(current_bcc_edges);
                    bcc_nodes.push_back(current_bcc_nodes);
                    bcc_count++;
                }
            } 
            else {
                low[u] = min(low[u], dfn[v]);
            }
        }

        // Special condition for DFS Root node to be an Articulation Point
        if (p_edge_id == -1 && children_count > 1) {
            is_cut_vertex[u] = true;
        }
    }

    void run() {
        for (int i = 1; i <= n; i++) {
            if (dfn[i] == -1) {
                // Handle Isolated Nodes (Node with degree 0)
                if (adj[i].empty()) {
                    bcc_count++;
                    bcc_nodes.push_back({i});
                    bcc_edges.push_back({});
                    dfn[i] = low[i] = timer++;
                    continue;
                }
                dfs(i);
            }
        }

        // Collect list of Cut Vertices
        for (int i = 1; i <= n; i++) {
            if (is_cut_vertex[i]) {
                cut_vertices.push_back(i);
            }
        }
    }

    // Builds the Block-Cut Tree
    void build_block_cut_tree() {
        // Size: Original Nodes (1..n) + Block Nodes (n+1 .. n+bcc_count)
        int total_bct_nodes = n + bcc_count;
        bct.assign(total_bct_nodes + 1, {});

        for (int i = 0; i < bcc_count; i++) {
            int block_id = n + 1 + i; // Block Node ID in BCT
            
            for (int u : bcc_nodes[i]) {
                if (is_cut_vertex[u]) {
                    // Connect Cut Vertex directly to the Block
                    bct[u].push_back(block_id);
                    bct[block_id].push_back(u);
                }
            }
        }
    }

    void reset(int new_n) {
        init(new_n);
    }
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int vertices = 7;
    TarjanBCC graph(vertices);

    // BCC 1: Cycle {1, 2, 3}
    graph.add_edge(1, 2);
    graph.add_edge(2, 3);
    graph.add_edge(3, 1);

    // Cut Vertex: Node 3
    graph.add_edge(3, 4); // Bridge BCC: Edge {3, 4}

    // Cut Vertex: Node 4
    // BCC 3: Cycle {4, 5, 6}
    graph.add_edge(4, 5);
    graph.add_edge(5, 6);
    graph.add_edge(6, 4);

    // Node 7 is isolated
    
    graph.run();
    graph.build_block_cut_tree();

    cout << "--- Articulation Points ---\n";
    for (int v : graph.cut_vertices) {
        cout << "Node " << v << " is a Cut Vertex\n";
    }

    cout << "\n--- Biconnected Components (Blocks) ---\n";
    cout << "Total BCCs found: " << graph.bcc_count << '\n';
    for (int i = 0; i < graph.bcc_count; i++) {
        cout << "Block " << i + 1 << " Nodes: ";
        for (int node : graph.bcc_nodes[i]) cout << node << ' ';
        cout << '\n';
    }

    cout << "\n--- Block-Cut Tree Adjacency ---\n";
    for (int v : graph.cut_vertices) {
        cout << "Cut Vertex " << v << " connects to BCT Blocks: ";
        for (int block_node : graph.bct[v]) {
            cout << block_node - graph.n << " (BCT Node " << block_node << ") ";
        }
        cout << '\n';
    }

    return 0;
}
