#include <bits/stdc++.h>
using namespace std;

const int INF = 1e9;

// ============================================================================
// DIAL'S ALGORITHM
// Usage: Fast Dijkstra when max edge weight C is very small (e.g., C <= 10).
// Time: O(V * C + E) | Space: O(V * C + E)
// ============================================================================
vector<int> dialDijkstra(int start, int n, int max_w, const vector<vector<pair<int, int>>>& g) {
    vector<int> dist(n + 1, INF);
    int max_dist = max_w * n;
    vector<vector<int>> buckets(max_dist + 1);

    dist[start] = 0;
    buckets[0].push_back(start);

    int idx = 0;
    while (idx <= max_dist) {
        while (idx <= max_dist && buckets[idx].empty()) idx++;
        if (idx > max_dist) break;

        int u = buckets[idx].back();
        buckets[idx].pop_back();

        for (auto [v, w] : g[u]) {
            if (dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                buckets[dist[v]].push_back(v);
            }
        }
    }
    return dist;
}

// Test Main
int main() {
    int n = 4, max_w = 3;
    vector<vector<pair<int, int>>> g(n + 1);

    g[1].push_back({2, 1});
    g[2].push_back({3, 2});
    g[1].push_back({3, 5});
    g[3].push_back({4, 1});

    vector<int> dist = dialDijkstra(1, n, max_w, g);
    cout << "Distance 1 -> 4: " << dist[4] << " (Expected: 4)\n";

    return 0;
}
