#include <bits/stdc++.h>
using namespace std;

/*
 * =============================================================================
 * TEMPLATE 3: Merge Sort Tree
 * =============================================================================
 * USAGE:
 * - Each segment tree node stores a sorted std::vector<int> of its range elements.
 * - Answers range frequency and counting queries without modifying array values.
 *
 * WHEN TO USE (Problem Types):
 * 1. Range query: "Count elements strictly greater than X in range [L, R]".
 * 2. Range query: "Count elements in range [X, Y] within index bounds [L, R]".
 * 3. Static array queries where elements need to be binary searched per range.
 *
 * COMPLEXITY:
 * - Time: Build O(N log N), Query O(log^2 N)
 * - Space: O(N log N)
 * =============================================================================
 */

struct MergeSortTree {
#define mid ((lx + rx) >> 1)
#define LT (node * 2 + 1)
#define RT (node * 2 + 2)

    int n;
    vector<vector<int>> tree;

    MergeSortTree(int sz) {
        n = sz;
        tree.resize(4 * n + 5);
    }

    void build(int node, int lx, int rx, const vector<int>& a) {
        if (lx == rx) {
            if (lx < (int)a.size()) tree[node] = {a[lx]};
            return;
        }
        build(LT, lx, mid, a);
        build(RT, mid + 1, rx, a);

        // Merge two sorted children in O(Length)
        merge(tree[LT].begin(), tree[LT].end(),
              tree[RT].begin(), tree[RT].end(),
              back_inserter(tree[node]));
    }

    void build(const vector<int>& a) {
        build(0, 0, n - 1, a);
    }

    // Counts elements strictly greater than 'val' in range [l, r]
    int query_greater(int node, int lx, int rx, int l, int r, int val) {
        if (rx < l || lx > r) return 0;
        if (lx >= l && rx <= r) {
            // Binary search over the sorted vector in current node
            return tree[node].end() - upper_bound(tree[node].begin(), tree[node].end(), val);
        }

        return query_greater(LT, lx, mid, l, r, val) +
               query_greater(RT, mid + 1, rx, l, r, val);
    }

    int query_greater(int l, int r, int val) {
        return query_greater(0, 0, n - 1, l, r, val);
    }

#undef mid
#undef LT
#undef RT
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    vector<int> a = {7, 3, 9, 1, 5, 2, 8, 4};
    MergeSortTree mst(a.size());
    mst.build(a);

    // Query: Count elements strictly greater than 4 in range [1, 6]
    // Subarray [1..6] is {3, 9, 1, 5, 2, 8} -> Elements > 4 are {9, 5, 8} -> Ans = 3
    int ans = mst.query_greater(1, 6, 4);

    cout << "=== MERGE SORT TREE ===\n";
    cout << "Elements > 4 in range [1, 6]: " << ans << " (Expected: 3)\n";

    return 0;
}
