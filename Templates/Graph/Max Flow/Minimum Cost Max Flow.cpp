#include <bits/stdc++.h>
using namespace std;

struct Edge {
    int to;
    int rev_idx;
    long long cap;
    long long cost;
    long long flow;
    int id;
};

struct MinCostMaxFlowPrimalDual {
    int n;
    vector<vector<Edge>> adj;
    vector<long long> dist;
    vector<long long> potential; // Node potentials to force positive weights
    vector<int> parent_node;
    vector<int> parent_edge_idx;
    
    struct EdgeInfo {
        int u, v;
        long long cap;
        long long cost;
    };
    vector<EdgeInfo> original_edges;

    MinCostMaxFlowPrimalDual(int n) : n(n), adj(n), dist(n), potential(n, 0), parent_node(n), parent_edge_idx(n) {}

    void add_edge(int u, int v, long long cap, long long cost) {
        int u_idx = adj[u].size();
        int v_idx = adj[v].size();
        adj[u].push_back({v, v_idx, cap, cost, 0, (int)original_edges.size()});
        adj[v].push_back({u, u_idx, 0, -cost, 0, -1});
        original_edges.push_back({u, v, cap, cost});
    }

    // Standard SPFA run ONLY ONCE to initialize potentials if there are negative costs initially
    void init_potentials(int s) {
        fill(potential.begin(), potential.end(), 1e18);
        vector<bool> in_q(n, false);
        queue<int> q;
        potential[s] = 0;
        q.push(s);
        in_q[s] = true;

        while (!q.empty()) {
            int u = q.front(); q.pop();
            in_q[u] = false;
            for (auto& edge : adj[u]) {
                if (edge.cap > 0 && potential[edge.to] > potential[u] + edge.cost) {
                    potential[edge.to] = potential[u] + edge.cost;
                    if (!in_q[edge.to]) {
                        q.push(edge.to);
                        in_q[edge.to] = true;
                    }
                }
            }
        }
    }

    // Fast Dijkstra using potentials to guarantee non-negative edge weights
    bool dijkstra(int s, int t) {
        fill(dist.begin(), dist.end(), 1e18);
        priority_queue<pair<long long, int>, vector<pair<long long, int>>, greater<pair<long long, int>>> pq;
        
        dist[s] = 0;
        pq.push({0, s});

        while (!pq.empty()) {
            auto [d, u] = pq.top();
            pq.pop();

            if (d > dist[u]) continue;

            for (int i = 0; i < adj[u].size(); ++i) {
                auto& edge = adj[u][i];
                // Reduced cost formula: cost + potential[u] - potential[to]
                long long reduced_cost = edge.cost + potential[u] - potential[edge.to];
                
                if (edge.cap - edge.flow > 0 && dist[edge.to] > dist[u] + reduced_cost) {
                    dist[edge.to] = dist[u] + reduced_cost;
                    parent_node[edge.to] = u;
                    parent_edge_idx[edge.to] = i;
                    pq.push({dist[edge.to], edge.to});
                }
            }
        }
        return dist[t] != 1e18;
    }

    // Main execution function
    pair<long long, long long> get_min_cost_max_flow(int s, int t, bool has_negative_edges = false) {
        if (has_negative_edges) {
            init_potentials(s);
        } else {
            fill(potential.begin(), potential.end(), 0);
        }

        long long max_flow = 0;
        long long min_cost = 0;

        while (dijkstra(s, t)) {
            // Update potentials using the shortest paths found
            for (int i = 0; i < n; ++i) {
                if (dist[i] != 1e18) potential[i] += dist[i];
            }

            // Find bottleneck capacity
            long long path_flow = 1e18;
            for (int curr = t; curr != s; curr = parent_node[curr]) {
                int p = parent_node[curr];
                int idx = parent_edge_idx[curr];
                path_flow = min(path_flow, adj[p][idx].cap - adj[p][idx].flow);
            }

            // Push flow along the path
            for (int curr = t; curr != s; curr = parent_node[curr]) {
                int p = parent_node[curr];
                int idx = parent_edge_idx[curr];
                
                adj[p][idx].flow += path_flow;
                adj[curr][adj[p][idx].rev_idx].flow -= path_flow;
                min_cost += path_flow * adj[p][idx].cost;
            }
            max_flow += path_flow;
        }
        return {max_flow, min_cost};
    }


    vector<long long> get_final_edge_flows() {
    vector<long long> flows(original_edges.size());
    for (int u = 0; u < n; ++u) {
        for (auto& edge : adj[u]) {
            if (edge.id != -1) { // التأكد أن الإيدج أصلي وليس وهمياً أو عكسياً
                flows[edge.id] = edge.flow;
            }
        }
    }
    return flows;
}
};

void solve() {
    /*
    ===========================================================================
    WHEN TO USE THIS:
    Use this version when performance matters. The Primal-Dual structure scales 
    beautifully on larger networks because Dijkstra runs incredibly fast compared 
    to SPFA's worst-case bounds.

    HOW TO USE THIS:
    1. Initialize with `MinCostMaxFlowPrimalDual solver(N);`
    2. Add edges using `solver.add_edge(u, v, capacity, cost);`
    3. Call `solver.get_min_cost_max_flow(source, sink, false);` 
       (Pass `true` as the third parameter ONLY if your initial edges have negative costs).
    ===========================================================================
    */
    int n, m;
    if (!(cin >> n >> m)) return;

    MinCostMaxFlowPrimalDual solver(n);

    for (int i = 0; i < m; ++i) {
        int u, v;
        long long cap, cost;
        cin >> u >> v >> cap >> cost;
        solver.add_edge(u, v, cap, cost);
    }

    int source = 0, sink = n - 1;
    auto [flow, cost] = solver.get_min_cost_max_flow(source, sink, false);

    cout << "Max Flow: " << flow << "\n";
    cout << "Min Cost: " << cost << "\n";
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    solve();
    return 0;
}
