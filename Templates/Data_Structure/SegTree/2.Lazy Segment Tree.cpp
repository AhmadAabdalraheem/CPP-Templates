#include <bits/stdc++.h>
using namespace std;

/*
 * =============================================================================
 * TEMPLATE: Dual Lazy Segment Tree (Range Add + Range Set + Range Query)
 * =============================================================================
 * Time Complexity : Build O(N), Query/Update O(log N)
 * Space Complexity: O(4 * N)
 * Indexing        : 0-indexed [0, n - 1]
 * =============================================================================
 *
 * IMPORTANT NOTES FOR TRD:
 * ------------------------
 * 1. Priority Rule: Assignment (Set) OVERRIDES previous Additions.
 *    Subsequent Additions ACCUMULATE on top of the active Assignment.
 *
 * 2. Lazy State:
 *    - has_set[node] = true  -> There is a pending assignment.
 *    - lazy_set[node]        -> Value to assign to the whole segment.
 *    - lazy_add[node]        -> Value to add after the assignment.
 *
 * 3. If a SET operation happens:
 *    - All previous ADD operations are discarded.
 *    - lazy_set becomes the new assigned value.
 *    - lazy_add becomes 0.
 *
 * 4. If an ADD operation happens:
 *    - If there is an active SET, the ADD is accumulated into lazy_set.
 *    - Otherwise, it is accumulated into lazy_add.
 *
 * 5. Range Operations:
 *    - Sum Query: Uses `val * (rx - lx + 1)`.
 *    - Min/Max Query: Remove `* (rx - lx + 1)` and update `neutral`.
 * =============================================================================
 */

struct LazySegmentTree {
#define mid ((lx + rx) >> 1)
#define LT (node * 2 + 1)
#define RT (node * 2 + 2)

    int n;

    // Neutral element:
    // 0 for Sum
    // 1e18 for Min
    // -1e18 for Max
    long long neutral = 0;
    vector<long long> seg;

    // Pending range addition.
    vector<long long> lazy_add;

    // Pending range assignment.
    vector<long long> lazy_set;

    // Whether this node has a pending SET operation.
    vector<bool> has_set;

    LazySegmentTree(int sz) {
        n = sz;
        seg.assign(4 * n + 5, 0);
        lazy_add.assign(4 * n + 5, 0);
        lazy_set.assign(4 * n + 5, 0);
        has_set.assign(4 * n + 5, false);
    }
    // -------------------------------------------------------------------------
    // Merge two child nodes.
    // For Sum: a + b
    // Change for Min/Max/GCD.
    // -------------------------------------------------------------------------
    long long merge(long long a, long long b) {
        return a + b;
    }
    // -------------------------------------------------------------------------
    // Apply SET operation to the current node.
    //
    // SET completely overrides previous ADD operations.
    // -------------------------------------------------------------------------
    void apply_set(int node, int lx, int rx, long long val) {
        seg[node] = val * (rx - lx + 1);

        lazy_set[node] = val;
        lazy_add[node] = 0;
        has_set[node] = true;
    }
    // -------------------------------------------------------------------------
    // Apply ADD operation to the current node.
    //
    // If there is already a pending SET:
    //      SET x + ADD y  -> SET (x + y)
    //
    // Otherwise:
    //      ADD y is stored in lazy_add.
    // -------------------------------------------------------------------------
    void apply_add(int node, int lx, int rx, long long val) {
        seg[node] += val * (rx - lx + 1);

        if (has_set[node]) {
            lazy_set[node] += val;
        } else {
            lazy_add[node] += val;
        }
    }
    // -------------------------------------------------------------------------
    // Push lazy operations from the current node to its children.
    //
    // SET has priority and is pushed first.
    // Then ADD is pushed.
    // -------------------------------------------------------------------------
    void push(int node, int lx, int rx) {
        if (lx == rx) return;

        // Push pending SET first.
        if (has_set[node]) {
            apply_set(LT, lx, mid, lazy_set[node]);
            apply_set(RT, mid + 1, rx, lazy_set[node]);

            lazy_set[node] = 0;
            has_set[node] = false;
        }
        // Push pending ADD after SET.
        if (lazy_add[node] != 0) {
            apply_add(LT, lx, mid, lazy_add[node]);
            apply_add(RT, mid + 1, rx, lazy_add[node]);

            lazy_add[node] = 0;
        }
    }
    // -------------------------------------------------------------------------
    // Build the segment tree.
    // -------------------------------------------------------------------------
    void build(int node, int lx, int rx, const vector<int>& a) {
        if (lx == rx) {
            if (lx < (int)a.size())
                seg[node] = a[lx];

            return;
        }

        build(LT, lx, mid, a);
        build(RT, mid + 1, rx, a);

        seg[node] = merge(seg[LT], seg[RT]);
    }
    // -------------------------------------------------------------------------
    // Public build function.
    // -------------------------------------------------------------------------
    void build(const vector<int>& a) {
        build(0, 0, n - 1, a);
    }
    // -------------------------------------------------------------------------
    // Generic range update.
    //
    // is_set = true  -> SET
    // is_set = false -> ADD
    // -------------------------------------------------------------------------
    void update_range(
        int node,
        int lx,
        int rx,
        int l,
        int r,
        long long val,
        bool is_set
    ) {
        // No intersection.
        if (rx < l || lx > r)
            return;

        // Complete intersection.
        if (lx >= l && rx <= r) {
            if (is_set)
                apply_set(node, lx, rx, val);
            else
                apply_add(node, lx, rx, val);

            return;
        }

        // Propagate lazy operations before going down.
        push(node, lx, rx);

        update_range(LT, lx, mid, l, r, val, is_set);
        update_range(RT, mid + 1, rx, l, r, val, is_set);

        // Recalculate current node.
        seg[node] = merge(seg[LT], seg[RT]);
    }

    // -------------------------------------------------------------------------
    // Range ADD:
    // Add val to every element in [l, r].
    // -------------------------------------------------------------------------
    void update_add(int l, int r, long long val) {
        update_range(0, 0, n - 1, l, r, val, false);
    }

    // -------------------------------------------------------------------------
    // Range SET:
    // Set every element in [l, r] to val.
    // -------------------------------------------------------------------------
    void update_set(int l, int r, long long val) {
        update_range(0, 0, n - 1, l, r, val, true);
    }

    // -------------------------------------------------------------------------
    // Range Query.
    // Returns the SUM of elements in [l, r].
    // -------------------------------------------------------------------------
    long long query(
        int node,
        int lx,
        int rx,
        int l,
        int r
    ) {
        // No intersection.
        if (rx < l || lx > r)
            return neutral;

        // Complete intersection.
        if (lx >= l && rx <= r)
            return seg[node];

        // Push lazy operations before querying children.
        push(node, lx, rx);

        return merge(
            query(LT, lx, mid, l, r),
            query(RT, mid + 1, rx, l, r)
        );
    }

    // -------------------------------------------------------------------------
    // Public query function.
    // -------------------------------------------------------------------------
    long long query(int l, int r) {
        return query(0, 0, n - 1, l, r);
    }

#undef mid
#undef LT
#undef RT
};



// ==================== USAGE EXAMPLE ====================
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    vector<int> a = {1, 2, 3, 4, 5};
    LazySegmentTree st(a.size());
    st.build(a);

    // Initial sum of entire range -> 15
    cout << "Initial Sum [0, 4]: " << st.query(0, 4) << "\n";

    // 1. Range Addition: Add 5 to range [0, 2]
    // Array becomes: {6, 7, 8, 4, 5}
    st.update_add(0, 2, 5);
    cout << "Sum [0, 2] after adding 5: " << st.query(0, 2) << "\n"; // 6+7+8 = 21

    // 2. Range Assignment: Set range [1, 3] to 10
    // Array becomes: {6, 10, 10, 10, 5}
    st.update_set(1, 3, 10);
    cout << "Sum [0, 4] after setting [1, 3] to 10: " << st.query(0, 4) << "\n"; // 6+10+10+10+5 = 41

    // 3. Range Addition on top of Assignment: Add 2 to range [2, 4]
    // Array becomes: {6, 10, 12, 12, 7}
    st.update_add(2, 4, 2);
    cout << "Final Sum [0, 4]: " << st.query(0, 4) << "\n"; // 6+10+12+12+7 = 47

    return 0;
}
