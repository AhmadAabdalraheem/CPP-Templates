#include <bits/stdc++.h>
using namespace std;

using ll = long long;
const ll oo = 2e18;

// ============================================================================
// MULTI-SOURCE DIJKSTRA
// Usage: Shortest path to the NEAREST source among a set of source nodes.
// Time: O((V + E) log V) | Space: O(V + E)
// ============================================================================
vector<ll> multiSourceDijkstra(int n, const vector<int>& sources, const vector<vector<pair<int, ll>>>& g) {
    vector<ll> dist(n + 1, oo);
    priority_queue<pair<ll, int>, vector<pair<ll, int>>, greater<pair<ll, int>>> pq;

    // Push ALL source nodes simultaneously with distance 0
    for (int src : sources) {
        dist[src] = 0;
        pq.push({0, src});
    }

    while (!pq.empty()) {
        auto [cost, u] = pq.top();
        pq.pop();

        if (cost > dist[u]) continue;

        for (auto [v, w] : g[u]) {
            if (dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                pq.push({dist[v], v});
            }
        }
    }
    return dist; // dist[i] = shortest distance to ANY source
}

// Test Main
int main() {
    int n = 5;
    vector<vector<pair<int, ll>>> g(n + 1);

    // Graph connections
    g[1].push_back({3, 10}); g[2].push_back({3, 2});
    g[3].push_back({4, 1});  g[4].push_back({5, 5});

    vector<int> sources = {1, 2}; // Multiple starting points
    vector<ll> dist = multiSourceDijkstra(n, sources, g);

    cout << "Shortest dist to node 4 from any source: " << dist[4] << " (Expected: 3 via source 2)\n";
    return 0;
}
