#include <bits/stdc++.h>
#define all(v) v.begin() , v.end()
#define allr(v) v.rbegin() , v.rend()
using namespace std ;
using ll = long long ;
#define int ll
const int oo = 2e18;
const int N =2e6+5;

struct Edge {
    int to;
    int rev_idx;    // Index of the reverse edge in adj[to]
    long long cap;  // Remaining capacity
    long long flow; // Current flow online
    int id;         // ID of the original edge (-1 for virtual edges)
};

struct FlowWithDemands {
    int n;
    int S, T; // Virtual source and sink
    long long total_demands = 0;

    vector<vector<Edge>> adj;
    vector<int> level;
    vector<int> ptr;
    vector<long long> balance; // Balance of each node (Inflow - Outflow demands)

    struct EdgeInfo {
        int u, v;
        long long lower;
        long long upper;
    };
    vector<EdgeInfo> original_edges;

    FlowWithDemands(int n) : n(n), adj(n + 2), level(n + 2), ptr(n + 2), balance(n + 2, 0) {
        S = n;
        T = n + 1;
    }

    // Add edge with a minimum requirement (lower) and maximum capacity (upper)
    int add_edge(int u, int v, long long lower, long long upper, bool is_infinite_edge = false) {
        int u_idx = adj[u].size();
        int v_idx = adj[v].size();

        int edge_id = is_infinite_edge ? -1 : (int)original_edges.size();

        adj[u].push_back({v, v_idx, upper - lower, 0, edge_id});
        adj[v].push_back({u, u_idx, 0, 0, -1});

        balance[u] -= lower;
        balance[v] += lower;

        if (!is_infinite_edge) {
            original_edges.push_back({u, v, lower, upper});
        }

        return u_idx; // Return position in adj[u]
    }

    bool bfs() {
        fill(level.begin(), level.end(), -1);
        level[S] = 0;
        queue<int> q;
        q.push(S);
        while (!q.empty()) {
            int v = q.front();
            q.pop();
            for (auto& edge : adj[v]) {
                if (edge.cap - edge.flow > 0 && level[edge.to] == -1) {
                    level[edge.to] = level[v] + 1;
                    q.push(edge.to);
                }
            }
        }
        return level[T] != -1;
    }

    long long dfs(int v, long long pushed) {
        if (pushed == 0 || v == T) return pushed;
        for (int& cid = ptr[v]; cid < adj[v].size(); ++cid) {
            auto& edge = adj[v][cid];
            int tr = edge.to;
            if (level[v] + 1 != level[tr] || edge.cap - edge.flow == 0) continue;

            long long tr_pushed = dfs(tr, min(pushed, edge.cap - edge.flow));
            if (tr_pushed == 0) continue;

            edge.flow += tr_pushed;
            adj[tr][edge.rev_idx].flow -= tr_pushed;
            return tr_pushed;
        }
        return 0;
    }

    long long dinic_max_flow() {
        long long flow = 0;
        while (bfs()) {
            fill(ptr.begin(), ptr.end(), 0);
            while (long long pushed = dfs(S, 1e18)) {
                flow += pushed;
            }
        }
        return flow;
    }

    // STEP 1: Check if any valid solution exists that satisfies all min demands
    bool check_if_valid_solution_exists() {
        total_demands = 0;
        for (int i = 0; i < n; ++i) {
            if (balance[i] > 0) {
                int s_idx = adj[S].size();
                int i_idx = adj[i].size();
                adj[S].push_back({i, i_idx, balance[i], 0, -1});
                adj[i].push_back({S, s_idx, 0, 0, -1});
                total_demands += balance[i];
            } else if (balance[i] < 0) {
                int i_idx = adj[i].size();
                int t_idx = adj[T].size();
                adj[i].push_back({T, t_idx, -balance[i], 0, -1});
                adj[T].push_back({i, i_idx, 0, 0, -1});
            }
        }
        long long max_flow_built = dinic_max_flow();
        return (max_flow_built == total_demands);
    }

    // STEP 2: Find MAX flow from real source 's' to real sink 't'
    long long get_max_flow_with_demands(int s, int t) {
        // Add an infinite loop edge from t to s
        int inf_edge_idx = add_edge(t, s, 0, 1e18, true);

        if (!check_if_valid_solution_exists()) {
            return -1; // Not possible to satisfy demands
        }

        // The current flow on the (t -> s) edge is our feasible flow
        long long initial_flow = adj[t][inf_edge_idx].flow;

        // Deactivate the (t -> s) infinite edge for residual augmentation
        adj[t][inf_edge_idx].cap = 0;
        adj[t][inf_edge_idx].flow = 0;
        int rev_s_idx = adj[t][inf_edge_idx].rev_idx;
        adj[s][rev_s_idx].cap = 0;
        adj[s][rev_s_idx].flow = 0;

        // Direct Dinic from real source 's' to real sink 't'
        S = s;
        T = t;
        long long extra_flow = dinic_max_flow();

        return initial_flow + extra_flow;
    }

    // Get the final real flow passing through each original edge
    vector<long long> get_final_edge_flows() {
        vector<long long> flows(original_edges.size());
        for (int u = 0; u < n; ++u) {
            for (auto& edge : adj[u]) {
                if (edge.id != -1) {
                    flows[edge.id] = original_edges[edge.id].lower + edge.flow;
                }
            }
        }
        return flows;
    }
};

void solve () {
    // example usage
    // int n, m;
    // cin>>n>>m;
    //
    // FlowWithDemands solver(n);
    //
    // for (int i = 0; i < m; ++i) {
    //     int u, v;
    //     long long lower, upper;
    //     cin >> u >> v >> lower >> upper;
    //     --u; --v; // 0-based indexing
    //     solver.add_edge(u, v, lower, upper);
    // }
    //
    // if (solver.check_if_valid_solution_exists()) {
    //     cout << "YES\n";
    //     vector<long long> flows = solver.get_final_edge_flows();
    //     for (int i = 0; i < m; ++i) {
    //         cout << flows[i] << "\n";
    //     }
    // } else {
    //     cout << "NO\n";
    // }



    
    // Example Graph with 4 nodes: 0 (Source), 1, 2, 3 (Sink)
    // 0 -> 1: demands [2, 5]
    // 0 -> 2: demands [1, 4]
    // 1 -> 3: demands [2, 3]
    // 2 -> 3: demands [1, 5]
    // 1 -> 2: demands [0, 2]
    int n = 4;
    FlowWithDemands solver(n);
    
    solver.add_edge(0, 1, 2, 5);
    solver.add_edge(0, 2, 1, 4);
    solver.add_edge(1, 3, 2, 3);
    solver.add_edge(2, 3, 1, 5);
    solver.add_edge(1, 2, 0, 2);
    
    int s = 0, t = 3;
    long long max_flow = solver.get_max_flow_with_demands(s, t);
    cout << "Max Flow with Demands (0 -> 3): " << max_flow << " (Expected: 8)\n";
    vector<long long> flows = solver.get_final_edge_flows();
    cout << "Edge Flows:\n";
    for (int i = 0; i < (int)flows.size(); i++) {
        cout << "Edge " << i << ": " << flows[i] << "\n";
    }

    //EXPECTED OUTPUT
    //Max Flow with Demands (0 -> 3): 8 (Expected: 7)
    // Edge Flows:
    // Edge 0: 4
    // Edge 1: 4
    // Edge 2: 3
    // Edge 3: 5
    // Edge 4: 1
}
signed main() {
    ios_base:: sync_with_stdio(false) , cin.tie(NULL) , cout.tie(NULL) ;
    int T  = 1;
    //cin >> T ;
    while (T--) {
        solve() ;
    }
}
