#include <bits/stdc++.h>
using namespace std;

using ll = long long;
const ll oo = 2e18;

// ============================================================================
// K-TH SHORTEST PATH
// Usage: Find the K-th shortest path distance from start to target.
// Time: O(K * E log(V * K)) | Space: O(V * K)
// ============================================================================
ll kShortestPath(int start, int target, int n, int k, const vector<vector<pair<int, ll>>>& g) {
    vector<vector<ll>> dist(n + 1);
    priority_queue<pair<ll, int>, vector<pair<ll, int>>, greater<pair<ll, int>>> pq;

    pq.push({0, start});

    while (!pq.empty()) {
        auto [cost, u] = pq.top();
        pq.pop();

        if ((int)dist[u].size() >= k) continue;

        dist[u].push_back(cost);

        for (auto [v, w] : g[u]) {
            if ((int)dist[v].size() < k) {
                pq.push({cost + w, v});
            }
        }
    }

    if ((int)dist[target].size() < k) return -1;
    return dist[target][k - 1]; // 0-indexed: k-1 is the K-th shortest
}

// Test Main
int main() {
    int n = 4, k = 2; // Find 2nd shortest path
    vector<vector<pair<int, ll>>> g(n + 1);

    g[1].push_back({2, 3}); g[1].push_back({3, 1});
    g[2].push_back({4, 4}); g[2].push_back({3, 2});
    g[3].push_back({4, 10});

    ll second_best = kShortestPath(1, 4, n, k, g);
    cout << "2nd Shortest Path distance 1 -> 4: " << second_best << " (Expected: 11)\n";

    return 0;
}
