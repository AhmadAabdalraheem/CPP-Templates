#include <bits/stdc++.h>
using namespace std;

const long long INF = 1e18;

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
    vector<long long> potential;
    vector<int> parent_node;
    vector<int> parent_edge_idx;
    int edge_count = 0;

    MinCostMaxFlowPrimalDual(int n) : n(n), adj(n), dist(n), potential(n, 0), parent_node(n), parent_edge_idx(n) {}

    void add_edge(int u, int v, long long cap, long long cost) {
        int u_idx = adj[u].size();
        int v_idx = adj[v].size();
        adj[u].push_back({v, v_idx, cap, cost, 0, edge_count});
        adj[v].push_back({u, u_idx, 0, -cost, 0, -1});
        edge_count++;
    }

    // Handles initial negative cost edges using SPFA across all components
    void init_potentials() {
        fill(potential.begin(), potential.end(), 0);
        vector<bool> in_q(n, true);
        queue<int> q;
        for (int i = 0; i < n; ++i) q.push(i);

        while (!q.empty()) {
            int u = q.front(); q.pop();
            in_q[u] = false;

            for (auto& edge : adj[u]) {
                if (edge.cap - edge.flow > 0 && potential[edge.to] > potential[u] + edge.cost) {
                    potential[edge.to] = potential[u] + edge.cost;
                    if (!in_q[edge.to]) {
                        q.push(edge.to);
                        in_q[edge.to] = true;
                    }
                }
            }
        }
    }

    // Early-exit Dijkstra using potentials
    bool dijkstra(int s, int t) {
        fill(dist.begin(), dist.end(), INF);
        priority_queue<pair<long long, int>, vector<pair<long long, int>>, greater<pair<long long, int>>> pq;

        dist[s] = 0;
        pq.push({0, s});

        while (!pq.empty()) {
            auto [d, u] = pq.top(); pq.pop();

            if (d > dist[u]) continue;
            if (u == t) break; // Early stop when sink is reached

            for (int i = 0; i < (int)adj[u].size(); ++i) {
                auto& edge = adj[u][i];
                long long reduced_cost = edge.cost + potential[u] - potential[edge.to];

                if (edge.cap - edge.flow > 0 && dist[edge.to] > dist[u] + reduced_cost) {
                    dist[edge.to] = dist[u] + reduced_cost;
                    parent_node[edge.to] = u;
                    parent_edge_idx[edge.to] = i;
                    pq.push({dist[edge.to], edge.to});
                }
            }
        }
        return dist[t] != INF;
    }

    pair<long long, long long> get_min_cost_max_flow(int s, int t, bool has_negative_edges = false) {
        if (has_negative_edges) {
            init_potentials();
        } else {
            fill(potential.begin(), potential.end(), 0);
        }

        long long max_flow = 0;
        long long min_cost = 0;

        while (dijkstra(s, t)) {
            long long d_t = dist[t];
            for (int i = 0; i < n; ++i) {
                potential[i] += min(dist[i], d_t);
            }

            long long path_flow = INF;
            for (int curr = t; curr != s; curr = parent_node[curr]) {
                int p = parent_node[curr];
                int idx = parent_edge_idx[curr];
                path_flow = min(path_flow, adj[p][idx].cap - adj[p][idx].flow);
            }

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

    // Decomposes flow into individual s-t paths
    vector<pair<long long, vector<int>>> get_flow_paths(int s, int t) {
        vector<pair<long long, vector<int>>> paths;
        vector<vector<Edge>> temp_adj = adj;

        while (true) {
            vector<int> path;
            vector<pair<int, int>> path_edges;
            vector<bool> visited(n, false);

            auto find_path = [&](auto& self, int u) -> long long {
                if (u == t) return INF;
                visited[u] = true;
                path.push_back(u);

                for (int i = 0; i < (int)temp_adj[u].size(); ++i) {
                    auto& edge = temp_adj[u][i];
                    if (edge.id != -1 && edge.flow > 0 && !visited[edge.to]) {
                        path_edges.push_back({u, i});
                        long long bottleneck = self(self, edge.to);
                        if (bottleneck > 0) return min(bottleneck, edge.flow);
                        path_edges.pop_back();
                    }
                }
                path.pop_back();
                return 0;
            };

            long long current_flow = find_path(find_path, s);
            if (current_flow == 0) break;

            path.push_back(t);
            paths.push_back({current_flow, path});

            for (auto& [u, idx] : path_edges) {
                temp_adj[u][idx].flow -= current_flow;
            }
        }
        return paths;
    }

    vector<long long> get_final_edge_flows() {
        vector<long long> flows(edge_count);
        for (int u = 0; u < n; ++u) {
            for (auto& edge : adj[u]) {
                if (edge.id != -1) flows[edge.id] = edge.flow;
            }
        }
        return flows;
    }
};
int main() {
    /*
       Test Graph Structure:
       Nodes: 0 (Source), 1, 2, 3 (Sink)

       Edges: (from -> to, capacity, cost)
       - 0 -> 1 : cap = 2, cost = 1
       - 0 -> 2 : cap = 1, cost = 2
       - 1 -> 2 : cap = 1, cost = 1
       - 1 -> 3 : cap = 1, cost = 3
       - 2 -> 3 : cap = 2, cost = 1

        Max Flow: 3
        Min Cost: 10

        Flow Decomposition Paths:
        Flow 1 via path: 0 -> 1 -> 2 -> 3
        Flow 1 via path: 0 -> 1 -> 3
        Flow 1 via path: 0 -> 2 -> 3
     */

    int nodes = 4;
    int source = 0, sink = 3;
    MinCostMaxFlowPrimalDual mcmf(nodes);

    mcmf.add_edge(0, 1, 2, 1);
    mcmf.add_edge(0, 2, 1, 2);
    mcmf.add_edge(1, 2, 1, 1);
    mcmf.add_edge(1, 3, 1, 3);
    mcmf.add_edge(2, 3, 2, 1);

    auto [max_flow, min_cost] = mcmf.get_min_cost_max_flow(source, sink);

    cout << "Max Flow: " << max_flow << "\n";
    cout << "Min Cost: " << min_cost << "\n";

    cout << "\nFlow Decomposition Paths:\n";
    auto paths = mcmf.get_flow_paths(source, sink);
    for (auto& [flow_val, path] : paths) {
        cout << "Flow " << flow_val << " via path: ";
        for (int i = 0; i < (int)path.size(); ++i) {
            cout << path[i] << (i + 1 == (int)path.size() ? "" : " -> ");
        }
        cout << "\n";
    }
}
