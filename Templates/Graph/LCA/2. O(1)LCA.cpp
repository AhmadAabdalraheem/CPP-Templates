#include <bits/stdc++.h>
using namespace std;

// ============================================================================
// O(1) LCA QUERY (EULER TOUR + SPARSE TABLE)
// Usage: Answers LCA queries in O(1) time after O(N log N) precomputation.
// Time: Precomputation O(N log N), Query O(1) | Space: O(N log N)
// ============================================================================
const int N = 2e5 + 5;
const int LOG = 20;

vector<int> adj_rmq[N];
int depth_rmq[N];
int first_occ[N];
vector<int> euler_tour;

int st[2 * N][LOG]; // Sparse table over Euler Tour
int lg2[2 * N];

void dfs_euler(int u, int p = 0, int d = 0) {
    depth_rmq[u] = d;
    first_occ[u] = euler_tour.size();
    euler_tour.push_back(u);

    for (int v : adj_rmq[u]) {
        if (v != p) {
            dfs_euler(v, u, d + 1);
            euler_tour.push_back(u); // Re-add parent on returning
        }
    }
}

void build_lca_rmq(int root, int n) {
    euler_tour.clear();
    dfs_euler(root, 0, 0);

    int m = euler_tour.size();

    // Log precomputation
    lg2[1] = 0;
    for (int i = 2; i <= m; i++) lg2[i] = lg2[i / 2] + 1;

    // Sparse Table Initialization
    for (int i = 0; i < m; i++) st[i][0] = euler_tour[i];

    for (int j = 1; (1 << j) <= m; j++) {
        for (int i = 0; i + (1 << j) <= m; i++) {
            int left = st[i][j - 1];
            int right = st[i + (1 << (j - 1))][j - 1];
            st[i][j] = (depth_rmq[left] < depth_rmq[right]) ? left : right;
        }
    }
}

int get_lca_fast(int u, int v) {
    int l = first_occ[u];
    int r = first_occ[v];
    if (l > r) swap(l, r);

    int len = r - l + 1;
    int k = lg2[len];

    int left = st[l][k];
    int right = st[r - (1 << k) + 1][k];

    return (depth_rmq[left] < depth_rmq[right]) ? left : right;
}

// Test Main
int main() {
    adj_rmq[1].push_back(2); adj_rmq[2].push_back(1);
    adj_rmq[1].push_back(3); adj_rmq[3].push_back(1);
    adj_rmq[2].push_back(4); adj_rmq[4].push_back(2);

    build_lca_rmq(1, 4);

    cout << "Fast LCA(4, 3): " << get_lca_fast(4, 3) << " (Expected: 1)\n";
    return 0;
}
