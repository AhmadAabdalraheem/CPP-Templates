#include <bits/stdc++.h>
using namespace std;

using ll = long long;
const ll oo = 2e18;

// ============================================================================
// STATE DIJKSTRA (K DISCOUNT COUPONS)
// Usage: Shortest path where up to K edges can be reduced to 0 weight.
// Time: O(K * (V + E) log(V * K)) | Space: O(V * K)
// ============================================================================
ll dijkstraKDiscounts(int start, int target, int n, int max_k, const vector<vector<pair<int, ll>>>& g) {
    // dist[node][k] = min cost to reach 'node' using 'k' coupons
    vector<vector<ll>> dist(n + 1, vector<ll>(max_k + 1, oo));
    
    // min-heap storing {cost, {node, k_used}}
    priority_queue<pair<ll, pair<int, int>>, vector<pair<ll, pair<int, int>>>, greater<pair<ll, pair<int, int>>>> pq;

    dist[start][0] = 0;
    pq.push({0, {start, 0}});

    while (!pq.empty()) {
        auto [cost, state] = pq.top();
        auto [u, k] = state;
        pq.pop();

        if (cost > dist[u][k]) continue;

        for (auto [v, w] : g[u]) {
            // Option 1: Normal edge step
            if (dist[u][k] + w < dist[v][k]) {
                dist[v][k] = dist[u][k] + w;
                pq.push({dist[v][k], {v, k}});
            }

            // Option 2: Use coupon (weight becomes 0)
            if (k < max_k && dist[u][k] < dist[v][k + 1]) {
                dist[v][k + 1] = dist[u][k];
                pq.push({dist[v][k + 1], {v, k + 1}});
            }
        }
    }

    ll ans = oo;
    for (int k = 0; k <= max_k; k++) ans = min(ans, dist[target][k]);
    return ans;
}

// Test Main
int main() {
    int n = 4, k = 1;
    vector<vector<pair<int, ll>>> g(n + 1);

    g[1].push_back({2, 10}); g[2].push_back({4, 10});
    g[1].push_back({3, 3});  g[3].push_back({4, 3});

    ll ans = dijkstraKDiscounts(1, 4, n, k, g);
    cout << "Min cost with 1 coupon 1 -> 4: " << ans << " (Expected: 3)\n";

    return 0;
}
