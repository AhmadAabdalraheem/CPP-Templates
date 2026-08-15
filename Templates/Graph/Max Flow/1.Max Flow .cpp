#include <bits/stdc++.h>
using namespace std;

/*
 * -----------------------------------------------------------------------------
 * TEMPLATE: Dinic's Algorithm with Hybrid (Directed/Undirected) & Min-Cut
 * -----------------------------------------------------------------------------
 * Time Complexity : O(V^2 * E) for Max Flow / Min Cut.
 * Space Complexity: O(V + E)
 * -----------------------------------------------------------------------------
 */

struct Dinic {
    struct Edge {
        int from;
        int to;
        long long cap;
        int rev;          // Index of the reverse edge in adj[to]
        bool is_original; // True if this edge was explicitly added by the user
    };
    struct FlowEdgeOutput {
        int from, to;
        long long flow;
        long long max_cap;
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
        //ai optimization
        //if (pushed_total == 0) level[u] = -1; // Optimization
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

    // Extract original edges that carried flow
    vector<FlowEdgeOutput> get_used_edges() {
        vector<FlowEdgeOutput> used_edges;
        for (int u = 0; u < n; u++) {
            for (auto &edge : adj[u]) {
                if (edge.is_original) {
                    long long actual_flow = adj[edge.to][edge.rev].cap;
                    long long original_cap = edge.cap + actual_flow;

                    if (actual_flow > 0) {
                        used_edges.push_back({edge.from, edge.to, actual_flow, original_cap});
                    }
                }
            }
        }
        return used_edges;
    }

    // Fast DFS-based path extraction with bottleneck computation O(E * V)
    vector<pair<long long, vector<int>>> get_max_flow_paths(int s, int t) {
        vector<FlowEdgeOutput> used = get_used_edges();
        vector<vector<pair<int, long long>>> flow_adj(n);
        for (auto &edge : used) {
            flow_adj[edge.from].push_back({edge.to, edge.flow});
        }

        vector<pair<long long, vector<int>>> paths;

        while (true) {
            vector<int> path;
            vector<bool> vis(n, false);

            auto dfs_path = [&](auto& self, int u, long long cur_flow) -> long long {
                if (u == t) {
                    path.push_back(u);
                    return cur_flow;
                }
                vis[u] = true;
                path.push_back(u);

                for (auto &edge : flow_adj[u]) {
                    int v = edge.first;
                    long long &rem_flow = edge.second;
                    if (!vis[v] && rem_flow > 0) {
                        long long pushed = self(self, v, min(cur_flow, rem_flow));
                        if (pushed > 0) {
                            rem_flow -= pushed;
                            return pushed;
                        }
                    }
                }
                path.pop_back();
                return 0;
            };

            long long pushed = dfs_path(dfs_path, s, 2e18);
            if (pushed == 0) break;
            paths.push_back({pushed, path});
        }
        return paths;
    }
};


void solve(){
   // Built-in Test Graph: 6 Nodes (0 to 5)
    // Source = 0, Sink = 5
    int vertices = 6;
    int S = 0, T = 5;

    Dinic flow(vertices);

    // Edges with capacities:
    // 0 -> 1 (cap 10), 0 -> 2 (cap 10)
    // 1 -> 2 (cap 2),  1 -> 3 (cap 4), 1 -> 4 (cap 8)
    // 2 -> 4 (cap 9)
    // 3 -> 5 (cap 10), 4 -> 5 (cap 10)
    flow.add_edge(0, 1, 10);
    flow.add_edge(0, 2, 10);
    flow.add_edge(1, 2, 2);
    flow.add_edge(1, 3, 4);
    flow.add_edge(1, 4, 8);
    flow.add_edge(2, 4, 9);
    flow.add_edge(3, 5, 10);
    flow.add_edge(4, 5, 10);

    cout << "=================== DINIC TEST RUN ===================\n";

    long long max_flow = flow.get_max_flow(S, T);
    cout << "Maximum Flow from " << S << " to " << T << " is: " << max_flow << " (Expected: 19)\n";
    cout << "-----------------------------------------------------\n";

    // 1. Extracted Routes
    auto routes = flow.get_max_flow_paths(S, T);
    cout << "Extracted Flow Paths:\n";
    for (auto &[f_val, path] : routes) {
        cout << "Path (Flow = " << f_val << "): ";
        for (int i = 0; i < (int)path.size(); i++) {
            cout << path[i] << (i == (int)path.size() - 1 ? "" : " -> ");
        }
        cout << "\n";
    }
    cout << "-----------------------------------------------------\n";

    // 2. Edges that carried flow
    cout << "Edges that carried flow:\n";
    vector<Dinic::FlowEdgeOutput> flows = flow.get_used_edges();
    for (auto &edge : flows) {
        cout << "Edge (" << edge.from << " -> " << edge.to << ") | "
             << "Flow: " << edge.flow << " / " << edge.max_cap << "\n";
    }
    cout << "-----------------------------------------------------\n";

    // 3. Min Cut Edges
    cout << "Edges forming the Minimum Cut:\n";
    vector<pair<int, int>> cut_edges = flow.get_min_cut_edges(S);
    for (auto &edge : cut_edges) {
        cout << "Cut Edge: " << edge.first << " -> " << edge.second << "\n";
    }
    cout << "-----------------------------------------------------\n";

    // 4. S-side and T-side Partitions
    auto [s_side, t_side] = flow.get_cut_sides(S);

    cout << "S-side components (Reachable from Source):\n";
    for (int node : s_side) cout << node << " ";

    cout << "\n\nT-side components (Cut off / Sink side):\n";
    for (int node : t_side) cout << node << " ";
    cout << "\n=====================================================\n";


    // =================== DINIC TEST RUN ===================
    // Maximum Flow from 0 to 5 is: 14 (Expected: 19)
    // -----------------------------------------------------
    // Extracted Flow Paths:
    // Path (Flow = 4): 0 -> 1 -> 3 -> 5
    // Path (Flow = 6): 0 -> 1 -> 4 -> 5
    // Path (Flow = 4): 0 -> 2 -> 4 -> 5
    // -----------------------------------------------------
    // Edges that carried flow:
    // Edge (0 -> 1) | Flow: 10 / 10
    // Edge (0 -> 2) | Flow: 4 / 10
    // Edge (1 -> 3) | Flow: 4 / 4
    // Edge (1 -> 4) | Flow: 6 / 8
    // Edge (2 -> 4) | Flow: 4 / 9
    // Edge (3 -> 5) | Flow: 4 / 10
    // Edge (4 -> 5) | Flow: 10 / 10
    // -----------------------------------------------------
    // Edges forming the Minimum Cut:
    // Cut Edge: 1 -> 3
    // Cut Edge: 4 -> 5
    // -----------------------------------------------------
    // S-side components (Reachable from Source):
    // 0 1 2 4 
    //
    // T-side components (Cut off / Sink side):
    // 3 5 
    // =====================================================
}

int main() {
    // Fast I/O for Competitive Programming
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int t = 1;
    // cin >> t; // Uncomment if the problem has multiple test cases
    while (t--) {
        solve();
    }
    return 0;
}
