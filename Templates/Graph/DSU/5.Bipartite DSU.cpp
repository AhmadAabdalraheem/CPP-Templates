#include <bits/stdc++.h>
using namespace std;

// ============================================================================
// BIPARTITE DSU
// Usage: Dynamic bipartite checking / odd cycle detection during edge additions.
// Time: O(alpha(N)) | Space: O(2N)
// ============================================================================
struct BipartiteDSU {
    vector<int> parent;
    int n;

    void init(int n_val) {
        n = n_val;
        parent.resize(2 * n + 1);
        iota(parent.begin(), parent.end(), 0);
    }

    int find_root(int u) {
        if (u == parent[u]) return u;
        return parent[u] = find_root(parent[u]);
    }

    // Adds edge (u, v) demanding u and v to have DIFFERENT colors
    bool add_edge(int u, int v) {
        int root_u = find_root(u);
        int root_v = find_root(v);

        // If u and v already belong to the same color set -> Odd cycle detected!
        if (root_u == root_v) return false;

        // u gets v's opposite color, v gets u's opposite color
        parent[find_root(u)] = find_root(v + n);
        parent[find_root(v)] = find_root(u + n);
        return true;
    }

    bool same_color(int u, int v) {
        return find_root(u) == find_root(v);
    }
};

// Test Main
int main() {
    BipartiteDSU dsu;
    dsu.init(4); // 4 nodes

    cout << "Add edge (1, 2): " << dsu.add_edge(1, 2) << " (Expected: 1)\n";
    cout << "Add edge (2, 3): " << dsu.add_edge(2, 3) << " (Expected: 1)\n";
    cout << "Add edge (3, 4): " << dsu.add_edge(3, 4) << " (Expected: 1)\n";

    // Adding edge (1, 3) makes cycle 1-2-3-1 (Odd length 3) -> Not Bipartite!
    cout << "Add edge (1, 3) [Odd Cycle]: " << dsu.add_edge(1, 3) << " (Expected: 0)\n";

    // Adding edge (1, 4) makes cycle 1-2-3-4-1 (Even length 4) -> Valid Bipartite!
    cout << "Add edge (1, 4) [Even Cycle]: " << dsu.add_edge(1, 4) << " (Expected: 1)\n";

    return 0;
}
