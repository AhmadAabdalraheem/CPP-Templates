#include <bits/stdc++.h>
using namespace std;

using ll = long long;
const ll oo = 2e18;
const int MOD = 1e9 + 7;

// ============================================================================
// COUNT SHORTEST PATHS (DIJKSTRA + DP)
// Usage: Finds distance AND number of ways to reach all nodes in shortest path.
// Time: O((V + E) log V) | Space: O(V)
// ============================================================================
pair<vector<ll>, vector<int>> countShortestPaths(int start, int n, const vector<vector<pair<int, ll>>>& g) {
    vector<ll> dist(n + 1, oo);
    vector<int> ways(n + 1, 0); // ways[i] = number of shortest paths to i
    
    priority_queue<pair<ll, int>, vector<pair<ll, int>>, greater<pair<ll, int>>> pq;

    dist[start] = 0;
    ways[start] = 1;
    pq.push({0, start});

    while (!pq.empty()) {
        auto [cost, u] = pq.top();
        pq.pop();

        if (cost > dist[u]) continue;

        for (auto [v, w] : g[u]) {
            // Found a strictly shorter path -> Reset ways
            if (dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                ways[v] = ways[u];
                pq.push({dist[v], v});
            } 
            // Found another path with EQUAL shortest distance -> Accumulate ways
            else if (dist[u] + w == dist[v]) {
                ways[v] = (ways[v] + ways[u]) % MOD;
            }
        }
    }
    return {dist, ways};
}

// Test Main
int main() {
    int n = 4;
    vector<vector<pair<int, ll>>> g(n + 1);

    // 1 -> 2 (cost 2), 1 -> 3 (cost 2), 2 -> 4 (cost 3), 3 -> 4 (cost 3)
    // 2 shortest paths 1 -> 4 with total weight = 5
    g[1].push_back({2, 2}); g[1].push_back({3, 2});
    g[2].push_back({4, 3}); g[3].push_back({4, 3});

    auto [dist, ways] = countShortestPaths(1, n, g);

    cout << "Shortest Distance 1 -> 4: " << dist[4] << " (Expected: 5)\n";
    cout << "Number of Shortest Paths: " << ways[4] << " (Expected: 2)\n";

    return 0;
}
