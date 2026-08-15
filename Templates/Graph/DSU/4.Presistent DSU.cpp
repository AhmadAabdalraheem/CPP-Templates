#include <bits/stdc++.h>
using namespace std;

// ============================================================================
// PARTIAL PERSISTENT DSU
// Usage: Query the exact time step 't' when u and v got connected.
// Time: Merge O(log N), Query O(log^2 N) | Space: O(N)
// ============================================================================
struct PersistentDSU {
    vector<int> parent, height, time;

    void init(int n) {
        parent.resize(n + 1);
        height.resize(n + 1, 1);
        time.resize(n + 1, 1e9); // Time when node was attached to parent
        iota(parent.begin(), parent.end(), 0);
    }

    int find_root(int node, int t) {
        if (parent[node] == node || time[node] > t) return node;
        return find_root(parent[node], t); // NO path compression!
    }

    bool merge(int u, int v, int t) {
        int root_u = find_root(u, t);
        int root_v = find_root(v, t);

        if (root_u == root_v) return false;

        if (height[root_u] > height[root_v]) swap(root_u, root_v);

        parent[root_u] = root_v;
        time[root_u] = t;
        if (height[root_u] == height[root_v]) height[root_v]++;

        return true;
    }

    // Binary search for min time u and v became connected
    int query_connected_time(int u, int v) {
        int low = 0, high = 1e9, ans = -1;
        while (low <= high) {
            int mid = low + (high - low) / 2;
            if (find_root(u, mid) == find_root(v, mid)) {
                ans = mid;
                high = mid - 1; // Try to find earlier time
            } else {
                low = mid + 1;
            }
        }
        return ans;
    }
};

// Test Main
int main() {
    PersistentDSU dsu;
    dsu.init(5);

    dsu.merge(1, 2, 10); // Connected at time t = 10
    dsu.merge(2, 3, 20); // Connected at time t = 20
    dsu.merge(4, 5, 30); // Connected at time t = 30

    cout << "Connected at t=15 (1, 3)? " << (dsu.find_root(1, 15) == dsu.find_root(3, 15)) << " (Expected: 0)\n";
    cout << "Connected at t=25 (1, 3)? " << (dsu.find_root(1, 25) == dsu.find_root(3, 25)) << " (Expected: 1)\n";
    
    cout << "Min time (1, 3) connected: " << dsu.query_connected_time(1, 3) << " (Expected: 20)\n";
    cout << "Min time (1, 5) connected: " << dsu.query_connected_time(1, 5) << " (Expected: -1)\n";

    return 0;
}
