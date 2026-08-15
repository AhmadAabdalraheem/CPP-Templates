#include <bits/stdc++.h>
using namespace std;

// Returns node at distance K from 'u' going up towards root
int get_kth_ancestor(int u, int k) {
    for (int j = 0; j < LOG; j++) {
        if ((k >> j) & 1) {
            u = up[u][j];
        }
    }
    return u;
}

// Find node after moving K steps on path u -> v
// (K = 0 returns u, K = dist(u,v) returns v)
int getNodeOnPath(int u, int v, int k) {
    int lca = get_lca(u, v);
    int d_u = depth[u] - depth[lca];
    int d_v = depth[v] - depth[lca];

    if (k <= d_u) {
        // Target node is on the ascending side (u -> LCA)
        return get_kth_ancestor(u, k);
    } else {
        // Target node is on the descending side (LCA -> v)
        int steps_from_v = d_v - (k - d_u);
        return get_kth_ancestor(v, steps_from_v);
    }
}
