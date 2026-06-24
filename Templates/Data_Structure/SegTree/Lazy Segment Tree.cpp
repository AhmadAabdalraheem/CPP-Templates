#include <bits/stdc++.h>
using namespace std;

struct LazySegmentTree {
#define mid ((lx + rx) >> 1)
#define LT (node * 2 + 1)
#define RT (node * 2 + 2)

    int n;
    long long neutral = 0; // Neutral element (e.g., 0 for Sum, 1e18 for Min)

    vector<long long> seg;
    vector<long long> lazy_add;
    vector<long long> lazy_set;
    vector<bool> has_set; // Tracks if there is a pending assignment (set) operation

    // Constructor
    LazySegmentTree(int sz) {
        n = sz;
        seg.assign(4 * n + 5, 0);
        lazy_add.assign(4 * n + 5, 0);
        lazy_set.assign(4 * n + 5, 0);
        has_set.assign(4 * n + 5, false);
    }

    // Merge logic (Change depending on the problem)
    long long merge(long long a, long long b) {
        return a + b;
    }

    // Helper to apply changes directly to a node
    void apply(int node, int lx, int rx, long long add_val, long long set_val, bool is_set) {
        if (is_set) {
            // Assignment overrides everything
            seg[node] = set_val * (rx - lx + 1);
            lazy_set[node] = set_val;
            lazy_add[node] = add_val; // Usually 0 or the new add value
            has_set[node] = true;
        } else {
            // Addition accumulates
            seg[node] += add_val * (rx - lx + 1);
            lazy_add[node] += add_val;
        }
    }

    // Push pending updates (both sets and adds) down to children
    void push(int node, int lx, int rx) {
        if (lx == rx) return; // Cannot push past leaf nodes

        // Push to Left Child
        if (has_set[node]) {
            apply(LT, lx, mid, lazy_add[node], lazy_set[node], true);
            apply(RT, mid + 1, rx, lazy_add[node], lazy_set[node], true);
        } else {
            apply(LT, lx, mid, lazy_add[node], 0, false);
            apply(RT, mid + 1, rx, lazy_add[node], 0, false);
        }

        // Clear current node's lazy state
        lazy_add[node] = 0;
        lazy_set[node] = 0;
        has_set[node] = false;
    }

    // Build the tree from an initial array
    void build(int node, int lx, int rx, vector<int>& a) {
        if (lx == rx) {
            seg[node] = a[lx];
            return;
        }
        build(LT, lx, mid, a);
        build(RT, mid + 1, rx, a);
        seg[node] = merge(seg[LT], seg[RT]);
    }

    void build(vector<int>& a) {
        build(0, 0, n - 1, a);
    }

    // Range Update (Type 1: Addition or Type 2: Assignment)
    // is_set = true for assignment, is_set = false for addition
    void update_range(int node, int lx, int rx, int l, int r, long long val, bool is_set) {
        if (rx < l || lx > r)
            return;

        if (lx >= l && rx <= r) {
            apply(node, lx, rx, (is_set ? 0 : val), (is_set ? val : 0), is_set);
            return;
        }

        push(node, lx, rx);

        update_range(LT, lx, mid, l, r, val, is_set);
        update_range(RT, mid + 1, rx, l, r, val, is_set);

        seg[node] = merge(seg[LT], seg[RT]);
    }

    // Public wrapper for range addition
    void update_add(int l, int r, long long val) {
        update_range(0, 0, n - 1, l, r, val, false);
    }

    // Public wrapper for range assignment
    void update_set(int l, int r, long long val) {
        update_range(0, 0, n - 1, l, r, val, true);
    }

    // Range Query
    long long query(int node, int lx, int rx, int l, int r) {
        if (rx < l || lx > r)
            return neutral;

        if (lx >= l && rx <= r)
            return seg[node];

        push(node, lx, rx);

        return merge(
            query(LT, lx, mid, l, r),
            query(RT, mid + 1, rx, l, r)
        );
    }

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
