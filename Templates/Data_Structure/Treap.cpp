#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <random>

using namespace std;

// ============================================================================
// BLACK BOX: Implicit Treap (Dynamic Array with Range Operations & Lazy Prop)
// ============================================================================
static mt19937_64 rng(1337);
const long long INF = 4e18;

struct ImplicitTreap {
    struct Node {
        long long val, sum, mx, mn;
        long long lazy_add, lazy_repl;
        bool has_repl, rev;
        int sz, l, r;
        uint64_t prior;

        Node(long long v = 0) {
            val = sum = mx = mn = v;
            lazy_add = lazy_repl = 0;
            has_repl = rev = false;
            sz = 1;
            l = r = 0;
            prior = rng();
        }
    };

    vector<Node> tree;
    int root;

    ImplicitTreap(int reserve_sz = 200005) {
        tree.reserve(reserve_sz);
        Node null_node;
        null_node.sz = 0;
        null_node.sum = 0;
        null_node.mx = -INF;
        null_node.mn = INF;
        tree.push_back(null_node); // Node 0 = NULL
        root = 0;
    }

    void clear() {
        tree.clear();
        Node null_node;
        null_node.sz = 0;
        null_node.sum = 0;
        null_node.mx = -INF;
        null_node.mn = INF;
        tree.push_back(null_node);
        root = 0;
    }

    inline int get_sz(int u) const { return tree[u].sz; }

    int new_node(long long val) {
        tree.push_back(Node(val));
        return (int)tree.size() - 1;
    }

    void apply_rev(int u) {
        if (!u) return;
        swap(tree[u].l, tree[u].r);
        tree[u].rev ^= 1;
    }

    void apply_repl(int u, long long v) {
        if (!u) return;
        tree[u].val = v;
        tree[u].mx = v;
        tree[u].mn = v;
        tree[u].sum = v * tree[u].sz;
        tree[u].lazy_repl = v;
        tree[u].has_repl = true;
        tree[u].lazy_add = 0;
    }

    void apply_add(int u, long long v) {
        if (!u) return;
        tree[u].val += v;
        tree[u].mx += v;
        tree[u].mn += v;
        tree[u].sum += v * tree[u].sz;
        tree[u].lazy_add += v;
    }

    void push_down(int u) {
        if (!u) return;
        if (tree[u].rev) {
            apply_rev(tree[u].l);
            apply_rev(tree[u].r);
            tree[u].rev = false;
        }
        if (tree[u].has_repl) {
            apply_repl(tree[u].l, tree[u].lazy_repl);
            apply_repl(tree[u].r, tree[u].lazy_repl);
            tree[u].has_repl = false;
            tree[u].lazy_repl = 0;
        }
        if (tree[u].lazy_add) {
            apply_add(tree[u].l, tree[u].lazy_add);
            apply_add(tree[u].r, tree[u].lazy_add);
            tree[u].lazy_add = 0;
        }
    }

    void push_up(int u) {
        if (!u) return;
        int l = tree[u].l, r = tree[u].r;
        tree[u].sz = 1 + tree[l].sz + tree[r].sz;
        tree[u].sum = tree[u].val + tree[l].sum + tree[r].sum;
        tree[u].mx = max({tree[u].val, tree[l].mx, tree[r].mx});
        tree[u].mn = min({tree[u].val, tree[l].mn, tree[r].mn});
    }

    void split(int u, int k, int &l, int &r) {
        if (!u) { l = r = 0; return; }
        push_down(u);
        if (get_sz(tree[u].l) >= k) {
            split(tree[u].l, k, l, tree[u].l);
            r = u;
            push_up(r);
        } else {
            split(tree[u].r, k - get_sz(tree[u].l) - 1, tree[u].r, r);
            l = u;
            push_up(l);
        }
    }

    void merge(int &u, int l, int r) {
        push_down(l); push_down(r);
        if (!l || !r) { u = l ? l : r; return; }
        if (tree[l].prior > tree[r].prior) {
            merge(tree[l].r, tree[l].r, r);
            u = l;
        } else {
            merge(tree[r].l, l, tree[r].l);
            u = r;
        }
        push_up(u);
    }

    // --- PUBLIC API (0-INDEXED) ---

    int size() const { return get_sz(root); }

    // Insert at index 'pos'
    void insert(int pos, long long val) {
        int l, r;
        split(root, pos, l, r);
        int node = new_node(val);
        merge(l, l, node);
        merge(root, l, r);
    }

    // Append to end
    void push_back(long long val) { insert(size(), val); }

    // Erase subsegment [ql, qr]
    void erase(int ql, int qr) {
        int l, mid, r;
        split(root, qr + 1, l, r);
        split(l, ql, l, mid);
        merge(root, l, r);
    }

    // Add value to range [ql, qr]
    void range_add(int ql, int qr, long long val) {
        int l, mid, r;
        split(root, qr + 1, l, r);
        split(l, ql, l, mid);
        apply_add(mid, val);
        merge(l, l, mid);
        merge(root, l, r);
    }

    // Replace range [ql, qr] with value
    void range_replace(int ql, int qr, long long val) {
        int l, mid, r;
        split(root, qr + 1, l, r);
        split(l, ql, l, mid);
        apply_repl(mid, val);
        merge(l, l, mid);
        merge(root, l, r);
    }

    // Reverse range [ql, qr]
    void reverse(int ql, int qr) {
        int l, mid, r;
        split(root, qr + 1, l, r);
        split(l, ql, l, mid);
        apply_rev(mid);
        merge(l, l, mid);
        merge(root, l, r);
    }

    // Cyclic shift range [ql, qr] right by k steps
    void cyclic_shift(int ql, int qr, int k, bool shift_left = false) {
        int len = qr - ql + 1;
        if (len <= 1) return;
        k %= len;
        if (k < 0) k += len;
        if (k == 0) return;
        if (shift_left) k = len - k;

        int l, mid, r, fh, sh;
        split(root, qr + 1, l, r);
        split(l, ql, l, mid);
        split(mid, len - k, fh, sh);
        merge(mid, sh, fh);
        merge(l, l, mid);
        merge(root, l, r);
    }

    struct QueryResult { long long sum, mx, mn; };

    // Range Query (Sum, Max, Min) on [ql, qr]
    QueryResult query(int ql, int qr) {
        int l, mid, r;
        split(root, qr + 1, l, r);
        split(l, ql, l, mid);
        QueryResult res = {tree[mid].sum, tree[mid].mx, tree[mid].mn};
        merge(l, l, mid);
        merge(root, l, r);
        return res;
    }

    // Print current array state
    void print(int u = -1) {
        if (u == -1) u = root;
        if (!u) return;
        push_down(u);
        print(tree[u].l);
        cout << tree[u].val << " ";
        print(tree[u].r);
        if (u == root) cout << "\n";
    }
};

// ============================================================================
// EXAMPLE USAGE IN MAIN
// ============================================================================
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    ImplicitTreap t;

    cout << "--- 1. Building Initial Array ---\n";
    for (int i = 1; i <= 6; i++) {
        t.push_back(i * 10); // [10, 20, 30, 40, 50, 60]
    }
    cout << "Array: "; t.print(); // Output: 10 20 30 40 50 60

    cout << "\n--- 2. Insertion & Deletion ---\n";
    t.insert(2, 99); // Insert 99 at index 2
    cout << "After insert(2, 99): "; t.print(); // Output: 10 20 99 30 40 50 60

    t.erase(1, 2); // Erase elements from index 1 to 2
    cout << "After erase(1, 2):  "; t.print(); // Output: 10 30 40 50 60

    cout << "\n--- 3. Range Updates ---\n";
    t.range_add(1, 3, 5); // Add +5 to range [1..3]
    cout << "After range_add(1, 3, +5): "; t.print(); // Output: 10 35 45 55 60

    t.range_replace(2, 4, 100); // Set range [2..4] to 100
    cout << "After range_replace(2, 4, 100): "; t.print(); // Output: 10 35 100 100 100

    cout << "\n--- 4. Structural Modifications ---\n";
    t.reverse(0, 3); // Reverse range [0..3]
    cout << "After reverse(0, 3): "; t.print(); // Output: 100 100 35 10 100

    t.cyclic_shift(1, 4, 1); // Right shift range [1..4] by 1
    cout << "After cyclic_shift(1, 4, 1): "; t.print(); // Output: 100 100 100 100 35

    cout << "\n--- 5. Range Query ---\n";
    auto q = t.query(1, 4); // Query range [1..4]
    cout << "Query [1..4] -> Sum: " << q.sum << ", Max: " << q.mx << ", Min: " << q.mn << "\n";

    return 0;
}
