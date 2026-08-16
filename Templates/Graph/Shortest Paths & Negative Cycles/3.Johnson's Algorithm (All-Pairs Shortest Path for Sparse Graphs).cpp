#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

/*
================================================================================
  PATTERN 3: Johnson's Algorithm (All-Pairs Shortest Path for Sparse Graphs)
================================================================================
  Problem Types & Trigger Criteria:
  - All-pairs shortest path with negative edge weights where V <= 2000 and E << V^2.
  - Avoids O(V^3) complexity of Floyd-Warshall on sparse directed graphs.
  - Time Complexity: O(V * E + V^2 log V), Space Complexity: O(V^2).
  
  Core Idea & Modifications:
  - Add auxiliary node S (0) connected to all vertices with weight 0.
  - Run SPFA from S to get node potentials h[u]. If negative cycle exists, abort.
  - Re-weight graph edges: w'(u, v) = w(u, v) + h[u] - h[v] >= 0.
  - Run Dijkstra V times using non-negative re-weighted edges w'.
  - Un-weight real shortest path distance: dist[u][v] = dist_dijkstra[u][v] - h[u] + h[v].
================================================================================
*/

namespace Johnsons {
    // --- DYNAMIC PART ---
    const long long INF = 1e18;

    struct Edge {
        int to;
        long long weight;
    };

    // --- FIXED ENGINE PART ---
    struct JohnsonsEngine {
        int n;
        vector<vector<Edge>> adj;
        vector<vector<long long>> dist;

        JohnsonsEngine(int nodes) : n(nodes), adj(nodes + 1),
                                    dist(nodes + 1, vector<long long>(nodes + 1, INF)) {}

        void add_edge(int u, int v, long long w) {
            adj[u].push_back({v, w});
        }

        bool compute() {
            int dummy = 0;
            vector<vector<Edge>> aug_adj = adj;
            aug_adj.resize(n + 1);

            // Add dummy vertex 0 connected to all 1..N vertices
            for (int i = 1; i <= n; ++i) {
                aug_adj[dummy].push_back({i, 0});
            }

            // Step 1: SPFA from dummy node to calculate potentials h
            vector<long long> h(n + 1, INF);
            vector<int> count(n + 1, 0);
            vector<bool> in_q(n + 1, false);
            queue<int> q;

            h[dummy] = 0;
            q.push(dummy);
            in_q[dummy] = true;

            while (!q.empty()) {
                int u = q.front();
                q.pop();
                in_q[u] = false;

                for (const auto& edge : aug_adj[u]) {
                    int v = edge.to;
                    long long w = edge.weight;
                    if (h[u] != INF && h[u] + w < h[v]) {
                        h[v] = h[u] + w;
                        if (!in_q[v]) {
                            q.push(v);
                            in_q[v] = true;
                            count[v]++;
                            if (count[v] > n + 1) return false; // Negative cycle detected
                        }
                    }
                }
            }

            // Step 2: Run Dijkstra from each vertex u using modified non-negative weights
            for (int u = 1; u <= n; ++u) {
                priority_queue<pair<long long, int>, vector<pair<long long, int>>, greater<pair<long long, int>>> pq;
                dist[u][u] = 0;
                pq.push({0, u});

                while (!pq.empty()) {
                    auto [d, curr] = pq.top();
                    pq.pop();

                    if (d > dist[u][curr]) continue;

                    for (const auto& edge : adj[curr]) {
                        int v = edge.to;
                        long long reweighted = edge.weight + h[curr] - h[v];

                        if (dist[u][curr] + reweighted < dist[u][v]) {
                            dist[u][v] = dist[u][curr] + reweighted;
                            pq.push({dist[u][v], v});
                        }
                    }
                }

                // Step 3: Convert distances back using potentials
                for (int v = 1; v <= n; ++v) {
                    if (dist[u][v] != INF) {
                        dist[u][v] = dist[u][v] - h[u] + h[v];
                    }
                }
            }

            return true;
        }
    };
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    Johnsons::JohnsonsEngine solver(4);
    solver.add_edge(1, 2, -2);
    solver.add_edge(2, 3, -1);
    solver.add_edge(3, 1, 4);
    solver.add_edge(3, 4, 2);
    solver.add_edge(1, 4, 3);

    if (solver.compute()) {
        for (int i = 1; i <= 4; ++i) {
            for (int j = 1; j <= 4; ++j) {
                if (solver.dist[i][j] == Johnsons::INF) cout << "INF ";
                else cout << solver.dist[i][j] << " ";
            }
            cout << "\n";
        }
    } else {
        cout << "Negative Cycle Detected!\n";
    }

    return 0;
}
