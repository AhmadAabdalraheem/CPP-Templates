#include <bits/stdc++.h>
using namespace std;

// Structure for each edge in the graph
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
    void add_edge(int u, int v, long long lower, long long upper) {
        int u_idx = adj[u].size();
        int v_idx = adj[v].size();
        
        // In residual graph: capacity becomes (upper - lower)
        adj[u].push_back({v, v_idx, upper - lower, 0, (int)original_edges.size()});
        adj[v].push_back({u, u_idx, 0, 0, -1}); 
        
        balance[u] -= lower; 
        balance[v] += lower;

        original_edges.push_back({u, v, lower, upper});
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
                adj[S].push_back({i, (int)adj[i].size(), balance[i], 0, -1});
                adj[i].push_back({S, (int)adj[S].size() - 1, 0, 0, -1});
                total_demands += balance[i];
            } else if (balance[i] < 0) {
                adj[i].push_back({T, (int)adj[T].size(), -balance[i], 0, -1});
                adj[T].push_back({i, (int)adj[i].size() - 1, 0, 0, -1});
            }
        }
        long long max_flow_built = dinic_max_flow();
        return (max_flow_built == total_demands);
    }

    // STEP 2 (Optional): Find MAX flow from a real source 's' to real sink 't'
    long long get_max_flow_with_demands(int s, int t) {
        // Add an infinite loop edge from t to s to make it a circulation problem
        add_edge(t, s, 0, 1e18); 
        
        if (!check_if_valid_solution_exists()) {
            return -1; // Not possible to satisfy demands
        }
        
        // The current flow from t to s is our initial valid flow
        long long initial_flow = adj[t].back().flow; 
        
        // Remove the virtual source S and sink T effects by running from s to t directly
        S = s; 
        T = t;
        long long extra_flow = dinic_max_flow();
        
        return initial_flow + extra_flow;
    }

    // STEP 3 (Optional): Find MIN flow from a real source 's' to real sink 't'
    long long get_min_flow_with_demands(int s, int t) {
        add_edge(t, s, 0, 1e18);
        if (!check_if_valid_solution_exists()) {
            return -1; // Not possible
        }
        
        long long initial_flow = adj[t].back().flow;
        
        // To minimize, we try to push flow BACKWARDS from t to s
        S = t;
        T = s;
        long long reduced_flow = dinic_max_flow();
        
        return initial_flow - reduced_flow;
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

void solve() {
    /*
    ===========================================================================
    WHEN TO USE THIS (متى نستخدم هذا الكود):
    1. circulation with demands: If the problem asks: "Is there any way to route 
       flow such that every edge gets at least its minimum required flow?"
    2. Max Flow with demands: "Find the maximum flow from s to t, but every 
       single edge must still satisfy its minimum required flow."
    3. Min Flow with demands: "Find the minimum flow from s to t needed to 
       keep the system running without violating minimum requirements."

    HOW TO USE THIS (كيفية الاستخدام):
    - Read N (nodes) and M (edges).
    - Loop M times, read u, v, lower_bound, upper_bound using cin >> u >> v.
    - Call the function that matches your problem type.
    ===========================================================================
    */

    int n, m; 
    if (!(cin >> n >> m)) return;

    FlowWithDemands solver(n);

    for (int i = 0; i < m; i++) {
        int u, v;
        long long min_required, max_capacity;
        cin >> u >> v >> min_required >> max_capacity;
        
        // If your graph is 1-indexed (nodes from 1 to N), uncomment the next lines:
        // u--; v--; 

        solver.add_edge(u, v, min_required, max_capacity);
    }

    // --- TYPE 1: Just check if a valid solution exists ---
    /*
    bool possible = solver.check_if_valid_solution_exists();
    if (!possible) {
        cout << "NO SOLUTION" << "\n";
    } else {
        cout << "YES SOLUTION FOUND!" << "\n";
        vector<long long> flows = solver.get_final_edge_flows();
        for (int i = 0; i < m; i++) {
            cout << "Edge " << i << " carries: " << flows[i] << "\n";
        }
    }
    */

    // --- TYPE 2: Max Flow with Demands between source 's' and sink 't' ---
    int s = 0, t = n - 1; // Change to your source and sink
    long long max_flow = solver.get_max_flow_with_demands(s, t);
    
    if (max_flow == -1) {
        cout << "Impossible to satisfy requirements." << "\n";
    } else {
        cout << "Maximum Flow possible: " << max_flow << "\n";
        
        // Print edge flows if required by the problem
        vector<long long> flows = solver.get_final_edge_flows();
        for (int i = 0; i < m; i++) {
            cout << "Edge " << i << " flow = " << flows[i] << "\n";
        }
    }
}

int main() {
    // Fast I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    solve();
    
    return 0;
}
