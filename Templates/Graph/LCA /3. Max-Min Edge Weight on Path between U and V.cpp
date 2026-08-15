#include <bits/stdc++.h>
using namespace std;

using ll = long long;
const int N = 2e5 + 5;
const int LOG = 20;

struct TreeEdge { int to; ll w; };
vector<TreeEdge> g_tree[N];

int up_node[N][LOG];
ll max_w[N][LOG]; // max_w[u][j] = max edge weight jumping 2^j steps from u
int depth_w[N];

void dfs_weights(int u, int p = 0, int d = 0, ll w_to_p = 0) {
    depth_w[u] = d;
    up_node[u][0] = p;
    max_w[u][0] = w_to_p;

    for (int j = 1; j < LOG; j++) {
        int ancestor = up_node[u][j - 1];
        up_node[u][j] = up_node[ancestor][j - 1];
        max_w[u][j] = max(max_w[u][j - 1], max_w[ancestor][j - 1]);
    }

    for (auto edge : g_tree[u]) {
        if (edge.to != p) {
            dfs_weights(edge.to, u, d + 1, edge.w);
        }
    }
}

// Query max edge weight on path between u and v
ll get_max_edge_on_path(int u, int v) {
    ll ans = 0;
    if (depth_w[u] < depth_w[v]) swap(u, v);

    // 1. Equalize depths
    for (int j = LOG - 1; j >= 0; j--) {
        if (depth_w[u] - (1 << j) >= depth_w[v]) {
            ans = max(ans, max_w[u][j]);
            u = up_node[u][j];
        }
    }

    if (u == v) return ans;

    // 2. Lift together
    for (int j = LOG - 1; j >= 0; j--) {
        if (up_node[u][j] != up_node[v][j]) {
            ans = max({ans, max_w[u][j], max_w[v][j]});
            u = up_node[u][j];
            v = up_node[v][j];
        }
    }

    // Include final step to LCA
    ans = max({ans, max_w[u][0], max_w[v][0]});
    return ans;
}

// Test Main
int main() {
    // Tree: 1-2(weight 5), 2-3(weight 10), 2-4(weight 3)
    g_tree[1].push_back({2, 5});  g_tree[2].push_back({1, 5});
    g_tree[2].push_back({3, 10}); g_tree[3].push_back({2, 10});
    g_tree[2].push_back({4, 3});  g_tree[4].push_back({2, 3});

    dfs_weights(1, 0, 0, 0);

    cout << "Max edge on path 3 -> 4: " << get_max_edge_on_path(3, 4) << " (Expected: 10)\n";
    cout << "Max edge on path 1 -> 4: " << get_max_edge_on_path(1, 4) << " (Expected: 5)\n";

    return 0;
}
