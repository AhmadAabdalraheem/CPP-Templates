#include <bits/stdc++.h>
using namespace std;

/*
 * =============================================================================
 * TEMPLATE 2: Dynamic (Implicit) Segment Tree
 * =============================================================================
 * USAGE:
 * - Operates on extremely large coordinate ranges (e.g., [0, 10^9] or [-10^18, 10^18]).
 * - Dynamically allocates nodes on demand without pre-allocating 4N space.
 *
 * WHEN TO USE (Problem Types):
 * 1. Online range updates/queries over large ranges (up to 10^9 or 10^18).
 * 2. When coordinate compression cannot be done offline beforehand.
 *
 * COMPLEXITY:
 * - Time: O(log(MAX_VAL)) per update or query
 * - Space: O(Q * log(MAX_VAL)) where Q is the number of updates
 * =============================================================================
 */

struct DynamicSegTree {
    struct Node {
        long long val = 0;
        int left = -1, right = -1;
    };

    long long L_BOUND, R_BOUND;
    vector<Node> tree;

    DynamicSegTree(long long l, long long r) : L_BOUND(l), R_BOUND(r) {
        tree.push_back(Node()); // Root is node 0
    }

    void add_left(int node) {
        if (tree[node].left == -1) {
            tree[node].left = tree.size();
            tree.push_back(Node());
        }
    }

    void add_right(int node) {
        if (tree[node].right == -1) {
            tree[node].right = tree.size();
            tree.push_back(Node());
        }
    }

    void update(int node, long long lx, long long rx, long long idx, long long val) {
        if (lx == rx) {
            tree[node].val += val;
            return;
        }
        long long mid = lx + (rx - lx) / 2;
        if (idx <= mid) {
            add_left(node);
            update(tree[node].left, lx, mid, idx, val);
        } else {
            add_right(node);
            update(tree[node].right, mid + 1, rx, idx, val);
        }

        long long left_val = (tree[node].left != -1) ? tree[tree[node].left].val : 0;
        long long right_val = (tree[node].right != -1) ? tree[tree[node].right].val : 0;
        tree[node].val = left_val + right_val;
    }

    void update(long long idx, long long val) {
        update(0, L_BOUND, R_BOUND, idx, val);
    }

    long long query(int node, long long lx, long long rx, long long l, long long r) {
        if (node == -1 || rx < l || lx > r) return 0;
        if (lx >= l && rx <= r) return tree[node].val;

        long long mid = lx + (rx - lx) / 2;
        return query(tree[node].left, lx, mid, l, r) +
               query(tree[node].right, mid + 1, rx, l, r);
    }

    long long query(long long l, long long r) {
        return query(0, L_BOUND, R_BOUND, l, r);
    }
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    // Sparse coordinate range from 0 to 1,000,000,000 (1e9)
    DynamicSegTree dst(0, 1e9);

    // Updates at extremely distant indices
    dst.update(100, 5);          // Add 5 at index 100
    dst.update(500000000, 10);   // Add 10 at index 500,000,000
    dst.update(1000000000, 20);  // Add 20 at index 1,000,000,000

    cout << "=== DYNAMIC SEGMENT TREE ===\n";
    // Range [0, 600,000,000] -> Sum = 5 + 10 = 15
    cout << "Sum [0, 600m]: " << dst.query(0, 600000000) << " (Expected: 15)\n";

    // Full Range [0, 1e9] -> Sum = 5 + 10 + 20 = 35
    cout << "Sum [0, 1e9]: " << dst.query(0, 1000000000) << " (Expected: 35)\n";

    return 0;
}
