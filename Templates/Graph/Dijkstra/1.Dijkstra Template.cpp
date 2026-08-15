#include <bits/stdc++.h>
using namespace std;

using ll = long long;
const int N = 1e5 + 5;
const ll oo = 2e18;

int n, m;
vector<pair<int, ll>> g[N]; // g[u] = {{v, weight}, ...}
vector<ll> dist;
vector<int> parent_node;

void dijkstra(int start) {
    dist.assign(n + 1, oo);
    parent_node.assign(n + 1, -1);

    // Min-heap storing {cost, node}
    priority_queue<pair<ll, int>, vector<pair<ll, int>>, greater<pair<ll, int>>> pq;
    
    dist[start] = 0;
    pq.push({0, start});

    while (!pq.empty()) {
        auto [cost, u] = pq.top();
        pq.pop();

        // Skip stale (outdated) nodes
        if (cost > dist[u]) continue;

        for (auto [v, w] : g[u]) {
            if (dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                parent_node[v] = u;
                pq.push({dist[v], v});
            }
        }
    }
}

void solve() {
    cin >> n >> m;

    // Reset graph for multiple test cases!
    for (int i = 1; i <= n; i++) {
        g[i].clear();
    }

    for (int i = 0; i < m; i++) {
        int u, v;
        ll w;
        cin >> u >> v >> w;
        g[u].push_back({v, w});
        g[v].push_back({u, w}); // Remove if graph is DIRECTED
    }

    dijkstra(1);

    // If destination is unreachable
    if (dist[n] == oo) {
        cout << -1 << "\n";
        return;
    }

    // Path Reconstruction
    vector<int> path;
    for (int cur = n; cur != -1; cur = parent_node[cur]) {
        path.push_back(cur);
    }
    reverse(path.begin(), path.end());

    for (int i = 0; i < (int)path.size(); i++) {
        cout << path[i] << (i == (int)path.size() - 1 ? "" : " ");
    }
    cout << "\n";
}

// Test Main
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int t = 1;
    // cin >> t; // Uncomment if multiple testcases
    while (t--) {
        solve();
    }
    return 0;
}
