#include <bits/stdc++.h>
using namespace std;

// ============================================================================
// DSU WITH ROLLBACK
// Usage: Dynamic connectivity with undo capability.
// Time: Find O(log N), Merge O(log N), Rollback O(1) | Space: O(N)
// ============================================================================
struct DSURollback {
    vector<int> parent, sz;
    
    struct Op { int u, v, sz_u, sz_v; };
    vector<Op> history;
    int num_components;

    void init(int n) {
        parent.resize(n + 1);
        sz.resize(n + 1, 1);
        iota(parent.begin(), parent.end(), 0);
        num_components = n;
        history.clear();
    }

    int find_root(int u) {
        while (u != parent[u]) u = parent[u]; // NO path compression!
        return u;
    }

    bool is_same_set(int u, int v) {
        return find_root(u) == find_root(v);
    }

    bool merge(int u, int v) {
        int root_u = find_root(u);
        int root_v = find_root(v);

        if (root_u == root_v) {
            history.push_back({-1, -1, 0, 0}); // Dummy merge
            return false;
        }

        if (sz[root_u] > sz[root_v]) swap(root_u, root_v);

        history.push_back({root_u, root_v, sz[root_u], sz[root_v]});
        parent[root_u] = root_v;
        sz[root_v] += sz[root_u];
        num_components--;
        return true;
    }

    void rollback() {
        if (history.empty()) return;
        auto [u, v, sz_u, sz_v] = history.back();
        history.pop_back();

        if (u == -1) return;

        parent[u] = u;
        sz[v] -= sz_u;
        num_components++;
    }

    int snapshot() { return history.size(); }

    void rollback_to(int snap) {
        while ((int)history.size() > snap) rollback();
    }
};

// Test Main
int main() {
    DSURollback dsu;
    dsu.init(5); // 5 nodes: 1, 2, 3, 4, 5

    dsu.merge(1, 2);
    dsu.merge(2, 3);
    cout << "Same set (1, 3): " << dsu.is_same_set(1, 3) << " (Expected: 1)\n";
    cout << "Components: " << dsu.num_components << " (Expected: 3)\n";

    int snap = dsu.snapshot(); // Take snapshot

    dsu.merge(4, 5);
    dsu.merge(3, 4); // Now 1, 2, 3, 4, 5 are all connected
    cout << "Same set (1, 5): " << dsu.is_same_set(1, 5) << " (Expected: 1)\n";
    cout << "Components: " << dsu.num_components << " (Expected: 1)\n";

    dsu.rollback_to(snap); // Undo last 2 merges (3-4 and 4-5)
    cout << "After Rollback to snapshot:\n";
    cout << "Same set (1, 5): " << dsu.is_same_set(1, 5) << " (Expected: 0)\n";
    cout << "Components: " << dsu.num_components << " (Expected: 3)\n";

    return 0;
}
