#include <bits/stdc++.h>

vector<int> dx  = { 1 , -1 , 0 , 0 ,  1 ,  1 , -1 , -1 };
vector<int> dy  = { 0 ,  0 , 1 , -1 ,  1 , -1 , 1 , -1 };

#define int ll
struct Dinic {
    struct Edge {
        int from;
        int to;
        long long cap;
        int rev;          // Index of the reverse edge in adj[to]
        bool is_original; // True if this edge was explicitly added by the user
    };

    int n;
    vector<vector<Edge>> adj;
    vector<int> level;
    vector<int> ptr;
    vector<bool> visited_in_cut;

    // Constructor to initialize the graph with N vertices
    Dinic(int n) : n(n), adj(n), level(n), ptr(n), visited_in_cut(n) {}

    // Function to clear the graph between multiple testcases
    void clear() {
        fill(level.begin(), level.end(), -1);
        fill(ptr.begin(), ptr.end(), 0);
        for (int i = 0; i < n; ++i) {
            adj[i].clear();
        }
    }

    /*
     * add_edge function:
     * - For Directed Graphs  : Set 'undirected = false' (Default). Reverse capacity is 0.
     * - For Undirected Graphs: Set 'undirected = true'. Reverse capacity equals forward capacity.
     */
    void add_edge(int u, int v, long long cap, bool undirected = false) {
        adj[u].push_back({u, v, cap, (int)adj[v].size(), true});
        adj[v].push_back({v, u, (undirected ? cap : 0), (int)adj[u].size() - 1, undirected});
    }

    // BFS to build the level graph
    bool bfs(int s, int t) {
        fill(level.begin(), level.end(), -1);
        level[s] = 0;
        queue<int> q;
        q.push(s);
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            for (auto &edge : adj[u]) {
                if (edge.cap > 0 && level[edge.to] == -1) {
                    level[edge.to] = level[u] + 1;
                    q.push(edge.to);
                }
            }
        }
        return level[t] != -1; // Returns true if the sink is reachable
    }

    // DFS to find blocking flows in the level graph
   long long dfs(int u, int t, long long pushed) {
    if (pushed == 0 || u == t) return pushed;

    long long pushed_total = 0;

    for (int &cid = ptr[u]; cid < adj[u].size(); ++cid) {
        auto &edge = adj[u][cid];
        int v = edge.to;

        if (level[u] + 1 != level[v] || edge.cap == 0) continue;

        long long tr = dfs(v, t, min(pushed, edge.cap));
        if (tr == 0) continue; // Skip if this path can't reach the sink

        // Update residual capacities
        edge.cap -= tr;
        adj[v][edge.rev].cap += tr;

        pushed_total += tr;
        pushed -= tr;

        // If the remaining capacity coming into 'u' is exhausted, we can stop
        if (pushed == 0) break;
    }

    // Optional optimization: If this node couldn't push anything at all,
    // isolate it so future DFS calls in this phase ignore it entirely.
    if (pushed_total == 0) level[u] = -1;

    return pushed_total;
}

    // Main function to calculate Maximum Flow (which equals Min Cut Value)
    long long get_max_flow(int s, int t) {
        long long flow = 0;
        while (bfs(s, t)) {
            fill(ptr.begin(), ptr.end(), 0);
            while (long long pushed = dfs(s, t, 2e18)) { // 2e18 behaves as Infinity
                flow += pushed;
            }
        }
        return flow;
    }

    // Function to extract the actual edges that form the Minimum Cut
    vector<pair<int, int>> get_min_cut_edges(int s) {
        fill(visited_in_cut.begin(), visited_in_cut.end(), false);
        queue<int> q;
        q.push(s);
        visited_in_cut[s] = true;

        // 1. Traverse reachable nodes from source using remaining residual capacity (cap > 0)
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            for (auto &edge : adj[u]) {
                if (edge.cap > 0 && !visited_in_cut[edge.to]) {
                    visited_in_cut[edge.to] = true;
                    q.push(edge.to);
                }
            }
        }

        // 2. Identify cut edges (Original edges going from a visited node to an unvisited node)
        vector<pair<int, int>> cut_edges;
        for (int u = 0; u < n; u++) {
            if (visited_in_cut[u]) {
                for (auto &edge : adj[u]) {
                    if (edge.is_original && !visited_in_cut[edge.to]) {
                        cut_edges.push_back({u, edge.to});
                    }
                }
            }
        }
        return cut_edges;
    }

    // Function to extract the full node partition of the Minimum Cut
    // Returns a pair: {S_side_nodes, T_side_nodes}
    pair<vector<int>, vector<int>> get_cut_sides(int s) {
        // 1. Run BFS to properly populate the visited_in_cut array based on residual capacities
        fill(visited_in_cut.begin(), visited_in_cut.end(), false);
        queue<int> q;
        q.push(s);
        visited_in_cut[s] = true;

        while (!q.empty()) {
            int u = q.front();
            q.pop();
            for (auto &edge : adj[u]) {
                if (edge.cap > 0 && !visited_in_cut[edge.to]) {
                    visited_in_cut[edge.to] = true;
                    q.push(edge.to);
                }
            }
        }

        // 2. Separate all vertices based on whether they were reachable or not
        vector<int> s_side, t_side;
        for (int i = 0; i < n; i++) {
            if (visited_in_cut[i]) {
                s_side.push_back(i);
            } else {
                t_side.push_back(i);
            }
        }
        return {s_side, t_side};
    }
    
};

int n , m, r, c;
bool isvalid(int x , int y) {
    return x>=0 && x<n && y>=0 && y<m;
}
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


signed main() {
    Abdalraheem();
    //freopen("input.txt", "r", stdin);
    //freopen("output.txt", "w", stdout);
    int tt = 1;
   // cin>>tt;
    while (tt--) {
        solve();
    }
}

