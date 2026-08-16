#include <iostream>
#include <vector>
#include <queue>
#include <numeric>

using namespace std;

/*
================================================================================
  PATTERN 1: System of Difference Constraints
================================================================================
  Problem Types & Trigger Criteria:
  - Systems of linear inequalities of the form x_i - x_j <= w_k.
  - Finding a feasible assignment for x_i or maximizing/minimizing x_N - x_1.
  - Keywords: "difference constraints", "range bounds", "relative difference".
  
  Core Idea & Modifications:
  - Rewrite x_i - x_j <= w as x_i <= x_j + w -> Directed edge j -> i with weight w.
  - Add virtual source S (0) connected to all vertices 1..N with edge weight 0.
  - Run SPFA from S. If negative cycle exists -> No feasible solution.
  - Otherwise, x_i = dist[i] is a valid solution.
================================================================================
*/

namespace DifferenceConstraints {
    // --- DYNAMIC PART ---
    const long long INF = 1e18;

    struct Edge {
        int to;
        long long weight;
    };

    // --- FIXED ENGINE PART ---
    struct SystemSolver {
        int n;
        vector<vector<Edge>> adj;
        vector<long long> dist;

        SystemSolver(int variables) : n(variables), adj(variables + 1), dist(variables + 1, INF) {}

        // Add inequality constraint: x_i - x_j <= w  =>  j -> i with weight w
        void add_constraint(int i, int j, long long w) {
            adj[j].push_back({i, w});
        }

        // Add equality constraint: x_i - x_j = w
        void add_exact_constraint(int i, int j, long long w) {
            add_constraint(i, j, w);
            add_constraint(j, i, -w);
        }

        bool solve(vector<long long>& result) {
            int src = 0;
            vector<vector<Edge>> aug_adj = adj;
            aug_adj.resize(n + 1);

            for (int i = 1; i <= n; ++i) {
                aug_adj[src].push_back({i, 0});
            }

            vector<int> count(n + 1, 0);
            vector<bool> in_queue(n + 1, false);
            queue<int> q;

            dist[src] = 0;
            q.push(src);
            in_queue[src] = true;

            while (!q.empty()) {
                int u = q.front();
                q.pop();
                in_queue[u] = false;

                for (const auto& edge : aug_adj[u]) {
                    int v = edge.to;
                    long long w = edge.weight;

                    if (dist[u] != INF && dist[u] + w < dist[v]) {
                        dist[v] = dist[u] + w;
                        if (!in_queue[v]) {
                            q.push(v);
                            in_queue[v] = true;
                            count[v]++;
                            if (count[v] > n + 1) return false; // Inconsistent system
                        }
                    }
                }
            }

            result.assign(dist.begin() + 1, dist.end());
            return true;
        }
    };
}

int main_system_demo() {
    DifferenceConstraints::SystemSolver system(3);
    // x1 - x2 <= 3  => 2 -> 1 (w=3)
    // x2 - x3 <= -2 => 3 -> 2 (w=-2)
    // x1 - x3 <= 0  => 3 -> 1 (w=0)
    system.add_constraint(1, 2, 3);
    system.add_constraint(2, 3, -2);
    system.add_constraint(1, 3, 0);

    vector<long long> solution;
    if (system.solve(solution)) {
        cout << "Feasible Solution Found:\n";
        for (int i = 0; i < 3; ++i) {
            cout << "x" << (i + 1) << " = " << solution[i] << "\n";
        }
    } else {
        cout << "No Solution (Inconsistent Constraints)\n";
    }
    return 0;
}
