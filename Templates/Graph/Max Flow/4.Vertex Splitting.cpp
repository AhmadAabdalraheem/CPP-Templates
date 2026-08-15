// ============================================================================
// VERTEX SPLITTING WRAPPER (FOR NODE CAPACITIES)
// Usage: Automatically splits each 1-indexed node u into u_in and u_out.
// Time: O(V^2 * E) | Space: O(V + E)
// ============================================================================
// NODES
struct VertexSplitDinic {
    int n;
    Dinic dinic;
    const long long INF = 2e18;

    // Helper mapping for 1-indexed nodes
    int in_node(int u)  { return 2 * u - 1; }
    int out_node(int u) { return 2 * u; }

    VertexSplitDinic(int n) : n(n), dinic(2 * n + 5) {}

    // Set capacity limit for Node 'u' (Creates internal edge u_in -> u_out)
    void set_node_capacity(int u, long long node_cap) {
        dinic.add_edge(in_node(u), out_node(u), node_cap);
    }

    // Add directed edge u -> v with capacity (Creates edge u_out -> v_in)
    void add_edge(int u, int v, long long edge_cap) {
        dinic.add_edge(out_node(u), in_node(v), edge_cap);
    }

    // Calculates Max Flow from Source 's' to Sink 't'
    long long get_max_flow(int s, int t) {
        return dinic.get_max_flow(in_node(s), out_node(t));
    }
};

// ============================================================================
// TEST MAIN
// ============================================================================
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    // Scenario: 4 Nodes (1-indexed)
    // 1 -> 2 (edge cap 10), 1 -> 3 (edge cap 10)
    // 2 -> 4 (edge cap 10), 3 -> 4 (edge cap 10)
    // Node 2 has a strict NODE CAPACITY bottleneck of 5!
    int n = 4;
    VertexSplitDinic flow(n);

    // Set Node Capacities:
    flow.set_node_capacity(1, 2e18); // Source node unlimited
    flow.set_node_capacity(2, 5);    // Node 2 is capped at 5!
    flow.set_node_capacity(3, 10);   // Node 3 is capped at 10
    flow.set_node_capacity(4, 2e18); // Sink node unlimited

    // Add Directed Edges:
    flow.add_edge(1, 2, 10);
    flow.add_edge(1, 3, 10);
    flow.add_edge(2, 4, 10);
    flow.add_edge(3, 4, 10);

    int source = 1, sink = 4;
    long long max_flow = flow.get_max_flow(source, sink);

    cout << "================ VERTEX SPLITTING TEST ================\n";
    cout << "Max Flow from Node " << source << " to Node " << sink << " is: " << max_flow << "\n";
    cout << "Expected Output: 15 (Path 1-2-4 gives 5 due to Node 2 limit, Path 1-3-4 gives 10)\n";
    cout << "=======================================================\n";

    return 0;
}
