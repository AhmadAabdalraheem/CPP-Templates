#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

/*
================================================================================
  PATTERN 1: SPFA / Bellman-Ford (Single Source Shortest Path & Negative Cycles)
================================================================================
  Problem Types & Trigger Criteria:
  - Single-source shortest path with negative edge weights.
  - Detecting negative weight cycles reachable from the source.
  - Propagating -INF to all nodes reachable from a negative cycle.
  - Time Complexity: O(V * E) worst-case, O(E) average for SPFA.
  
  Core Idea & Modifications:
  - Queue-based relaxation (SPFA). Track relaxation frequency per node (`count[u] >= V`).
  - If a negative cycle is detected, run a secondary BFS/DFS from cycle nodes to mark
    dist[u] = -INF for all affected downstream vertices.
================================================================================
*/

namespace SPFA {
    // --- DYNAMIC PART ---
    const long long INF = 1e18; // Use 1e18 to prevent overflow on additions

    struct Edge {
        int to;
        long long weight;
    };

    // --- FIXED ENGINE PART ---
    struct SPFAEngine {
        int n;
        vector<vector<Edge>> adj;
        vector<long long> dist;
        vector<int> parent;
        vector<int> count;
        vector<bool> in_queue;
        vector<bool> in_neg_cycle;

        SPFAEngine(int nodes) : n(nodes), adj(nodes + 1), dist(nodes + 1, INF),
                                parent(nodes + 1, -1), count(nodes + 1, 0),
                                in_queue(nodes + 1, false), in_neg_cycle(nodes + 1, false) {}

        void add_edge(int u, int v, long long w) {
            adj[u].push_back({v, w});
        }

        // Returns false if any negative cycle is reachable from src
        bool run(int src) {
            fill(dist.begin(), dist.end(), INF);
            fill(parent.begin(), parent.end(), -1);
            fill(count.begin(), count.end(), 0);
            fill(in_queue.begin(), in_queue.end(), false);
            fill(in_neg_cycle.begin(), in_neg_cycle.end(), false);

            queue<int> q;
            dist[src] = 0;
            q.push(src);
            in_queue[src] = true;

            queue<int> cycle_nodes;

            while (!q.empty()) {
                int u = q.front();
                q.pop();
                in_queue[u] = false;

                for (const auto& edge : adj[u]) {
                    int v = edge.to;
                    long long w = edge.weight;

                    if (dist[u] != INF && dist[u] + w < dist[v]) {
                        dist[v] = dist[u] + w;
                        parent[v] = u;
                        if (!in_queue[v]) {
                            q.push(v);
                            in_queue[v] = true;
                            count[v]++;
                            if (count[v] >= n) {
                                in_neg_cycle[v] = true;
                                cycle_nodes.push(v);
                            }
                        }
                    }
                }
            }

            if (cycle_nodes.empty()) return true; // No negative cycles reachable

            // Propagate -INF to all vertices affected by negative cycles
            queue<int> neg_q = cycle_nodes;
            while (!neg_q.empty()) {
                int u = neg_q.front();
                neg_q.pop();
                dist[u] = -INF;

                for (const auto& edge : adj[u]) {
                    int v = edge.to;
                    if (!in_neg_cycle[v]) {
                        in_neg_cycle[v] = true;
                        neg_q.push(v);
                    }
                }
            }
            return false;
        }

        vector<int> get_path(int target) {
            if (dist[target] == INF || dist[target] == -INF) return {};
            vector<int> path;
            for (int curr = target; curr != -1; curr = parent[curr]) {
                path.push_back(curr);
            }
            reverse(path.begin(), path.end());
            return path;
        }
    };
}

int main_spfa_demo() {
    SPFA::SPFAEngine solver(5);
    solver.add_edge(1, 2, 4);
    solver.add_edge(2, 3, -10);
    solver.add_edge(3, 4, 3);
    solver.add_edge(4, 2, 2); // Negative cycle: 2 -> 3 -> 4 -> 2 (weight: -5)
    solver.add_edge(4, 5, 5);

    bool no_cycle = solver.run(1);
    cout << "Reachable Negative Cycle: " << (no_cycle ? "No" : "Yes") << "\n";
    cout << "Distance to 5: " << (solver.dist[5] == -SPFA::INF ? "-INF" : to_string(solver.dist[5])) << "\n";
    return 0;
}
