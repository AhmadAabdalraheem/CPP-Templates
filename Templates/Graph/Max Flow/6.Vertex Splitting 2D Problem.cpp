  void solve(){
    cin>>n>>m>>r>>c;
    r--,c--;
    // We have n * m cells. Each cell has an In-node and an Out-node (Vertex Splitting).
    // Cell (i, j) linear index = i * m + j
    // In-node index = (cell idx) * 2
    // Out-node index = (cell idx) * 2 + 1
    int total_cells = n * m;
    int num_nodes = total_cells * 2 + 2;
    int S = num_nodes - 2; // Super Source
    int T = num_nodes - 1; // Super Sink

    vector<vector<int>> a(n, vector<int>(m));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            cin >> a[i][j];
        }
    }
    Dinic flow (num_nodes + 5);
    // Build the grid flow network topology
    for (int i =0 ;i<n ;i++) {
        for (int j =0 ; j <m ;j++) {
            int cell_idx = i*m + j;

            int in_node =cell_idx*2;
            int out_node = cell_idx*2 + 1;

            // 1. Establish internal edge representing vertex-split cost (In -> Out)
            if (i == r && j == c) {
                flow.add_edge(in_node,out_node,oo);  // Base cell cannot be obstructed
            }
            else {
                flow.add_edge(in_node,out_node,a[i][j]); // Capacity is the wall construction time
            }

            // 2. Connect grid boundaries directly to the Super Source (S -> In)
            if (i == 0 || i == n - 1 || j == 0 || j == m - 1) {
                flow.add_edge(S, in_node, oo);
            }

            // 3. Connect neighboring adjacent grid nodes (Out -> Neighbor's In)
            for (int k = 0 ; k < 4; k++) {
                int nx = i+dx[k];
                int ny = j+dy[k];

                if (isvalid(nx,ny)) {
                    int neg_idx = nx* m + ny;
                    int neg_in = neg_idx * 2;
                    flow.add_edge(out_node,neg_in,oo);// Inter-cell transition costs nothing (infinite capacity)
                }
            }
        }
    }
    // Establish the final link routing the base's entrance out to the terminal target (base_in -> T)
    int base_in = (r * m + c) * 2;
    flow.add_edge(base_in, T, oo);

    int min_cut = flow.get_max_flow(S,T);
    cout<<min_cut<<endl;



    // Post-Processing: Query the black-box API for cut edges to pinpoint structural wall nodes
    vector<pair<int, int>> cut_edges = flow.get_min_cut_edges(S);
    vector<bool> is_cell_cut(n * m, false);

    for (auto &edge : cut_edges) {
        int u = edge.first;
        int v = edge.second;
        // If a cut edge belongs to internal cell splitting (u is even and v is its consecutive odd pair)
        if (u % 2 == 0 && v == u + 1) {
            int cell_idx = u / 2;
            is_cell_cut[cell_idx] = true;
        }
    }

    for (int i = 0; i < n; i++) {
        string row_output = "";
        for (int j = 0; j < m; j++) {
            if (i == r && j == c) {
                row_output += ".";
                continue;
            }

            int cell_idx = i * m + j;

            if (is_cell_cut[cell_idx]) {
                row_output += "X";
            } else {
                row_output += ".";
            }
        }
        cout << row_output << endl;
    }
}
