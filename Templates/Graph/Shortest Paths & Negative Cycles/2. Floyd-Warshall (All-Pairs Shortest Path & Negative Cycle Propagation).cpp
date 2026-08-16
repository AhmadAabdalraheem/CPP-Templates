#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

/*
================================================================================
  PATTERN 2: Floyd-Warshall (All-Pairs Shortest Path & Negative Cycle Propagation)
================================================================================
  Problem Types & Trigger Criteria:
  - All-pairs shortest paths on dense or small graphs (N <= 500).
  - Transitive closure, min-max path capacity queries.
  - Full propagation of negative cycles across all affected vertex pairs.
  - Time Complexity: O(V^3), Space Complexity: O(V^2).
  
  Core Idea & Modifications:
  - DP formulation: dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]).
  - If dist[k][k] < 0 after the main algorithm, vertex k is inside a negative cycle.
  - Propagate -INF to dist[i][j] if path i -> j can route through any vertex k with dist[k][k] < 0.
================================================================================
*/

namespace FloydWarshall {
    // --- DYNAMIC PART ---
    const long long INF = 1e18;

    // --- FIXED ENGINE PART ---
    struct FloydEngine {
        int n;
        vector<vector<long long>> dist;
        vector<vector<int>> parent;

        FloydEngine(int nodes) : n(nodes), dist(nodes + 1, vector<long long>(nodes + 1, INF)),
                                 parent(nodes + 1, vector<int>(nodes + 1, -1)) {
            for (int i = 1; i <= n; ++i) {
                dist[i][i] = 0;
                parent[i][i] = i;
            }
        }

        void add_edge(int u, int v, long long w, bool directed = true) {
            if (w < dist[u][v]) {
                dist[u][v] = w;
                parent[u][v] = v;
            }
            if (!directed) {
                if (w < dist[v][u]) {
                    dist[v][u] = w;
                    parent[v][u] = u;
                }
            }
        }

        void compute() {
            // Main Floyd-Warshall DP
            for (int k = 1; k <= n; ++k) {
                for (int i = 1; i <= n; ++i) {
                    for (int j = 1; j <= n; ++j) {
                        if (dist[i][k] < INF && dist[k][j] < INF) {
                            if (dist[i][k] + dist[k][j] < dist[i][j]) {
                                dist[i][j] = dist[i][k] + dist[k][j];
                                parent[i][j] = parent[i][k];
                            }
                        }
                    }
                }
            }

            // Negative Cycle Propagation Pass
            for (int k = 1; k <= n; ++k) {
                if (dist[k][k] < 0) {
                    for (int i = 1; i <= n; ++i) {
                        for (int j = 1; j <= n; ++j) {
                            if (dist[i][k] < INF && dist[k][j] < INF) {
                                dist[i][j] = -INF;
                            }
                        }
                    }
                }
            }
        }

        vector<int> get_path(int u, int v) {
            if (dist[u][v] == INF || dist[u][v] == -INF) return {};
            vector<int> path;
            int curr = u;
            while (curr != v) {
                path.push_back(curr);
                curr = parent[curr][v];
                if (curr == -1) return {};
            }
            path.push_back(v);
            return path;
        }
    };
}

int main_floyd_demo() {
    FloydWarshall::FloydEngine solver(4);
    solver.add_edge(1, 2, 5);
    solver.add_edge(2, 3, 3);
    solver.add_edge(3, 1, -10); // Negative cycle 1 -> 2 -> 3 -> 1 (weight: -2)
    solver.add_edge(3, 4, 2);

    solver.compute();

    cout << "Dist 1 -> 4: " << (solver.dist[1][4] == -FloydWarshall::INF ? "-INF" : to_string(solver.dist[1][4])) << "\n";
    return 0;
}
