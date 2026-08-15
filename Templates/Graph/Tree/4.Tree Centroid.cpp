#include <bits/stdc++.h>
using namespace std;

// ============================================================================
// TREE CENTROID FINDER
// Usage: Finds the node that minimizes the maximum subtree size when removed.
// Time: O(N) | Space: O(N)
// ============================================================================
const int N = 2e5 + 5;

vector<int> g_cent[N];
int sz_c[N];

void dfs_sz(int u, int p) {
    sz_c[u] = 1;
    for (int v : g_cent[u]) {
        if (v != p) {
            dfs_sz(v, u);
            sz_c[u] += sz_c[v];
        }
    }
}

int find_centroid(int u, int p, int total_n) {
    for (int v : g_cent[u]) {
        if (v != p && sz_c[v] > total_n / 2) {
            return find_centroid(v, u, total_n);
        }
    }
    return u;
}

int getCentroid(int n) {
    dfs_sz(1, 0);
    return find_centroid(1, 0, n);
}

// Test Main
int main() {
    int n = 5;
    // Star tree: 1 connected to 2, 3, 4, 5
    g_cent[1] = {2, 3, 4, 5};
    g_cent[2] = {1}; g_cent[3] = {1}; g_cent[4] = {1}; g_cent[5] = {1};

    cout << "Tree Centroid: " << getCentroid(n) << " (Expected: 1)\n";
    return 0;
}
