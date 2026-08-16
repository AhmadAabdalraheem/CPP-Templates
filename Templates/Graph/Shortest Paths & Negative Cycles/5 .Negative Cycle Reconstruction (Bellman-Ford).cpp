#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

/*
================================================================================
  PATTERN 2: Negative Cycle Reconstruction (Bellman-Ford)
================================================================================
  Problem Types & Trigger Criteria:
  - Explicitly printing the sequence of vertices in a negative cycle.
  - Arbitrage detection in currency conversion graphs.
  - Time Complexity: O(V * E).
  
  Core Idea & Modifications:
  - Run Bellman-Ford for V iterations storing parent pointers.
  - Node `x` relaxed at iteration V is guaranteed to be in or reachable from a negative cycle.
  - Walk backwards V times (x = parent[x]) to ensure `x` is inside the cycle.
  - Trace backwards from `x` until returning to `x` to isolate the exact cycle loop.
================================================================================
*/

namespace NegativeCycleTrace {
    // --- DYNAMIC PART ---
    const long long INF = 1e18;

    struct Edge {
        int u, v;
        long long weight;
    };

    // --- FIXED ENGINE PART ---
    struct CycleDetector {
        int n;
        vector<Edge> edges;

        CycleDetector(int nodes) : n(nodes) {}

        void add_edge(int u, int v, long long w) {
            edges.push_back({u, v, w});
        }

        vector<int> find_negative_cycle() {
            vector<long long> dist(n + 1, 0); // Start dist=0 everywhere to check all components
            vector<int> parent(n + 1, -1);
            int last_relaxed = -1;

            for (int iter = 0; iter < n; ++iter) {
                last_relaxed = -1;
                for (const auto& edge : edges) {
                    if (dist[edge.u] + edge.weight < dist[edge.v]) {
                        dist[edge.v] = dist[edge.u] + edge.weight;
                        parent[edge.v] = edge.u;
                        last_relaxed = edge.v;
                    }
                }
            }

            if (last_relaxed == -1) return {}; // No negative cycle

            int cycle_node = last_relaxed;
            for (int i = 0; i < n; ++i) {
                cycle_node = parent[cycle_node];
            }

            vector<int> cycle;
            for (int curr = cycle_node;; curr = parent[curr]) {
                cycle.push_back(curr);
                if (curr == cycle_node && cycle.size() > 1) break;
            }

            reverse(cycle.begin(), cycle.end());
            return cycle;
        }
    };
}

int main_cycle_demo() {
    NegativeCycleTrace::CycleDetector solver(4);
    solver.add_edge(1, 2, 1);
    solver.add_edge(2, 3, -5);
    solver.add_edge(3, 4, 2);
    solver.add_edge(4, 2, 1); // Negative cycle: 2 -> 3 -> 4 -> 2 (weight: -2)

    vector<int> cycle = solver.find_negative_cycle();
    if (!cycle.empty()) {
        cout << "Negative Cycle Detected: ";
        for (int u : cycle) cout << u << " ";
        cout << "\n";
    }
    return 0;
}
