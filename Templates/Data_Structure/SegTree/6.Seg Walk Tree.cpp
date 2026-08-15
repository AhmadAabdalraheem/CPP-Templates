#include <bits/stdc++.h>
using namespace std;

/*
 * =============================================================================
 * TEMPLATE 4: Walk on Segment Tree (Binary Search on SegTree)
 * =============================================================================
 * USAGE:
 * - Finds the first/last index in range [L, R] satisfying a condition in O(log N).
 * - Avoids outer Binary Search + Range Query which costs O(log^2 N).
 *
 * WHEN TO USE (Problem Types):
 * 1. "Find the first index i >= L such that A[i] >= X".
 * 2. "Find the smallest prefix sum that is >= K".
 * 3. Range-constrained search problems where O(log N) efficiency is strict.
 *
 * COMPLEXITY:
 * - Time: O(log N) per query
 * - Space: O(4 * N)
 * =============================================================================
 */

struct SegTreeWalk {
#define mid ((lx + rx) >> 1)
#define LT (node * 2 + 1)
#define RT (node * 2 + 2)

    int n;
    vector<int> seg; // Stores maximum value in each segment

    SegTreeWalk(int sz) {
        n = sz;
        seg.assign(4 * n + 5, 0);
    }

    void update(int node, int lx, int rx, int idx, int val) {
        if (lx == rx) {
            seg[node] = val;
            return;
        }
        if (idx <= mid) update(LT, lx, mid, idx, val);
        else update(RT, mid + 1, rx, idx, val);

        seg[node] = max(seg[LT], seg[RT]);
    }

    void update(int idx, int val) {
        update(0, 0, n - 1, idx, val);
    }

    // Finds the first index in range [l, r] with value >= x
    int find_first(int node, int lx, int rx, int l, int r, int x) {
        if (rx < l || lx > r || seg[node] < x) return -1;
        if (lx == rx) return lx;

        int res = find_first(LT, lx, mid, l, r, x);
        if (res != -1) return res;
        return find_first(RT, mid + 1, rx, l, r, x);
    }

    int find_first(int l, int r, int x) {
        return find_first(0, 0, n - 1, l, r, x);
    }

#undef mid
#undef LT
#undef RT
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    vector<int> a = {2, 5, 1, 8, 4, 9, 3};
    SegTreeWalk st(a.size());

    for (int i = 0; i < (int)a.size(); i++) {
        st.update(i, a[i]);
    }

    // Find first index in range [0, 6] with value >= 6 -> Element 8 at index 3
    int idx1 = st.find_first(0, 6, 6);

    // Find first index in range [4, 6] with value >= 6 -> Element 9 at index 5
    int idx2 = st.find_first(4, 6, 6);

    cout << "=== WALK ON SEGMENT TREE ===\n";
    cout << "First index >= 6 in range [0, 6]: " << idx1 << " (Expected: 3)\n";
    cout << "First index >= 6 in range [4, 6]: " << idx2 << " (Expected: 5)\n";

    return 0;
}
