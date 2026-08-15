// ============================================================================
// 2D GRID VERTEX SPLITTER CLASS
// ============================================================================
struct Grid2DVertexSplitter {
    int rows, cols;
    int S, T; // Virtual Source and Sink
    Dinic dinic;
    const long long INF = 2e18;

    int dr[4] = {-1, 1, 0, 0};
    int dc[4] = {0, 0, -1, 1};

    // Indexing Helper Functions
    int cell_id(int r, int c) { return r * cols + c; }
    int in_node(int r, int c) { return 2 * cell_id(r, c); }
    int out_node(int r, int c) { return 2 * cell_id(r, c) + 1; }

    Grid2DVertexSplitter(int r, int c) : rows(r), cols(c), dinic(2 * r * c + 5) {
        S = 2 * r * c;
        T = 2 * r * c + 1;
    }

    bool is_valid(int r, int c) {
        return r >= 0 && r < rows && c >= 0 && c < cols;
    }

    // 1. Set individual cell capacity (Internal Edge: in_node -> out_node)
    void set_cell_capacity(int r, int c, long long cap) {
        dinic.add_edge(in_node(r, c), out_node(r, c), cap);
    }

    // 2. Connect neighbor cells (Grid Edge: out_node -> in_node of neighbor)
    void add_directed_edge(int r1, int c1, int r2, int c2, long long edge_cap = 2e18) {
        dinic.add_edge(out_node(r1, c1), in_node(r2, c2), edge_cap);
    }

    // 3. Connect all 4-directional neighbors automatically
    void connect_4dir_neighbors() {
        for (int r = 0; r < rows; r++) {
            for (int c = 0; c < cols; c++) {
                for (int i = 0; i < 4; i++) {
                    int nr = r + dr[i];
                    int nc = c + dc[i];
                    if (is_valid(nr, nc)) {
                        add_directed_edge(r, c, nr, nc, INF);
                    }
                }
            }
        }
    }

    // 4. Attach Super Source to cell (r, c)
    void add_source_cell(int r, int c, long long cap = 2e18) {
        dinic.add_edge(S, in_node(r, c), cap);
    }

    // 5. Attach cell (r, c) to Super Sink
    void add_sink_cell(int r, int c, long long cap = 2e18) {
        dinic.add_edge(out_node(r, c), T, cap);
    }

    // Solve Flow / Min-Cut between custom Source and Sink cells
    long long solve(int src_r, int src_c, int sink_r, int sink_c) {
        return dinic.get_max_flow(in_node(src_r, src_c), out_node(sink_r, sink_c));
    }

    // Solve Flow / Min-Cut using Super Source S and Super Sink T
    long long solve_super_st() {
        return dinic.get_max_flow(S, T);
    }
};

// ============================================================================
// TEST MAIN
// ============================================================================
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    // Problem Example:
    // Grid 3x3 where each cell has a removal cost.
    // Find min cost to stop path from Top-Left (0,0) to Bottom-Right (2,2).
    int R = 3, C = 3;
    Grid2DVertexSplitter grid(R, C);

    vector<vector<long long>> cell_cost = {
        {2e18, 5,    2e18},
        {2e18, 3,    2e18},
        {2e18, 10,   2e18}
    };

    // Set Cell Capacities
    for (int r = 0; r < R; r++) {
        for (int c = 0; c < C; c++) {
            grid.set_cell_capacity(r, c, cell_cost[r][c]);
        }
    }

    // Connect 4-directional grid transitions
    grid.connect_4dir_neighbors();

    // Min-Cut between Top-Left (0,0) and Bottom-Right (2,2)
    long long ans = grid.solve(0, 0, 2, 2);

    cout << "================ 2D GRID VERTEX SPLITTING ================\n";
    cout << "Min Cut Cost: " << ans << " (Expected: 18 -> Cells (0,1)=5 + (1,1)=3 + (2,1)=10)\n";
    cout << "=========================================================\n";

    return 0;
}
