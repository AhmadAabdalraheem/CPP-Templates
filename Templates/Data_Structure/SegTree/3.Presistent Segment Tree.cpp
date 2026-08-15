#include <bits/stdc++.h>
using namespace std;

/*
 * =============================================================================
 * TEMPLATE 1: Persistent Segment Tree (Historical Queries & Range K-th Smallest)
 * =============================================================================
 * USAGE:
 * - Maintains all historical versions of the array after each update.
 * - Allows querying historical states: "What was the range sum/max at time step T?".
 * - Finds the K-th smallest element in any subarray [L, R] in O(log N).
 *
 * WHEN TO USE (Problem Types):
 * 1. Historical Queries: "What was the range sum/max/min of subarray [L, R] at time step T?".
 * 2. Range K-th Element: "Find the K-th smallest number in subarray A[L...R]".
 * 3. Combined with Trees (LCA): "Find the K-th element on the simple path between U and V".
 *
 * COMPLEXITY:
 * - Time: Build O(N log N), Update & Query O(log N)
 * - Space: O(N log N + Q log N)
 * =============================================================================
 */

struct PersistentSegTree {
    struct Node {
        long long val;
        int left, right;
    };

    int n;
    vector<Node> tree;
    vector<int> roots; // roots[T] holds the root node index for version / time step T

    PersistentSegTree(int n) : n(n) {
        tree.push_back({0, 0, 0}); // Dummy node at index 0
        roots.push_back(build(0, n - 1)); // Initial version T = 0
    }

    int build(int lx, int rx) {
        int id = tree.size();
        tree.push_back({0, 0, 0});
        if (lx == rx) return id;

        int mid = (lx + rx) >> 1;
        tree[id].left = build(lx, mid);
        tree[id].right = build(mid + 1, rx);
        return id;
    }

    int update(int prev_root, int lx, int rx, int idx, long long val) {
        int id = tree.size();
        tree.push_back(tree[prev_root]); // Clone previous node state

        if (lx == rx) {
            tree[id].val += val; // Use = val for point assignment, += val for point addition
            return id;
        }

        int mid = (lx + rx) >> 1;
        if (idx <= mid)
            tree[id].left = update(tree[prev_root].left, lx, mid, idx, val);
        else
            tree[id].right = update(tree[prev_root].right, mid + 1, rx, idx, val);

        tree[id].val = tree[tree[id].left].val + tree[tree[id].right].val; // Change for Min/Max
        return id;
    }

    // Creates a new version/time step T by updating index 'idx'
    void update(int idx, long long val) {
        int new_root = update(roots.back(), 0, n - 1, idx, val);
        roots.push_back(new_root);
    }

    // -------------------------------------------------------------------------
    // 1. HISTORICAL RANGE QUERY AT TIME STEP T
    // -------------------------------------------------------------------------
    long long query_sum(int root, int lx, int rx, int l, int r) {
        if (rx < l || lx > r || root == 0) return 0;
        if (lx >= l && rx <= r) return tree[root].val;

        int mid = (lx + rx) >> 1;
        return query_sum(tree[root].left, lx, mid, l, r) +
               query_sum(tree[root].right, mid + 1, rx, l, r);
    }

    // Public Wrapper: Range Sum at specific historical time step T
    long long query_sum_at_time(int time_step_T, int l, int r) {
        return query_sum(roots[time_step_T], 0, n - 1, l, r);
    }

    // -------------------------------------------------------------------------
    // 2. RANGE K-TH SMALLEST ELEMENT QUERY
    // -------------------------------------------------------------------------
    int query_kth(int prev_root, int curr_root, int lx, int rx, int k) {
        if (lx == rx) return lx;

        int count_left = tree[tree[curr_root].left].val - tree[tree[prev_root].left].val;
        int mid = (lx + rx) >> 1;

        if (count_left >= k)
            return query_kth(tree[prev_root].left, tree[curr_root].left, lx, mid, k);
        else
            return query_kth(tree[prev_root].right, tree[curr_root].right, mid + 1, rx, k - count_left);
    }

    // Public Wrapper: Query K-th smallest in range using prefix versions
    int query_range_kth(int L_version, int R_version, int k) {
        return query_kth(roots[L_version], roots[R_version + 1], 0, n - 1, k);
    }
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n = 5;
    PersistentSegTree pst(n); // Initial array at Time T = 0: {0, 0, 0, 0, 0}

    // Time T = 1: Set A[1] += 10 -> Array: {0, 10, 0, 0, 0}
    pst.update(1, 10);

    // Time T = 2: Set A[3] += 5  -> Array: {0, 10, 0, 5, 0}
    pst.update(3, 5);

    // Time T = 3: Set A[1] += 20 -> Array: {0, 30, 0, 5, 0}
    pst.update(1, 20);

    cout << "=== HISTORICAL TIME STEP QUERIES ===\n";
    // Range sum [0, 4] at Time T = 1 -> Sum = 10
    cout << "Sum [0, 4] at Time T=1: " << pst.query_sum_at_time(1, 0, 4) << " (Expected: 10)\n";

    // Range sum [0, 4] at Time T = 2 -> Sum = 15
    cout << "Sum [0, 4] at Time T=2: " << pst.query_sum_at_time(2, 0, 4) << " (Expected: 15)\n";

    // Range sum [0, 4] at Time T = 3 -> Sum = 35
    cout << "Sum [0, 4] at Time T=3: " << pst.query_sum_at_time(3, 0, 4) << " (Expected: 35)\n";

    return 0;
}
