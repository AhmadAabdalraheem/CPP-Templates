#include <bits/stdc++.h>
using namespace std;

/*
 * =====================================================================
 * PATTERN 3: NON-COMMUTATIVE PATH QUERIES (DIRECTED u -> v)
 * =====================================================================
 * 
 * 📌 WHEN TO USE THIS TEMPLATE:
 * ---------------------------------------------------------------------
 * 1. The operation is NON-COMMUTATIVE: order matters! (A ⊕ B != B ⊕ A).
 *    Examples: Matrix Multiplication, String Concatenation, Maximum 
 *    Subarray Sum (Kadane on tree path), Maximum Increasing Subsequence.
 * 2. Direction of traversal matters: path (u -> v) is different from (v -> u).
 * 
 * 💡 CORE IDEA & IMPLEMENTATION DETAILS:
 * ---------------------------------------------------------------------
 * - Standard HLD swaps `u` and `v` randomly, which ruins order for 
 *   non-commutative operations.
 * - Solution: Split path u -> v into two separate ordered lists:
 *     1. Left Path: Segments climbing from `u` up to `LCA`.
 *     2. Right Path: Segments climbing from `v` up to `LCA`.
 * - Merge Left Path in order, then merge Right Path in REVERSE order.
 * - Requires segment tree to support querying reversed intervals if needed.
 * 
 * ⏱️ COMPLEXITIES:
 * ---------------------------------------------------------------------
 * - Build HLD: O(N)
 * - Directed Path Query: O(log^2 N)
 * - Space Complexity: O(N)
 * =====================================================================
 */

// =====================================================================
// 1. DYNAMIC PART (CUSTOMIZE FOR YOUR OPERATION)
// =====================================================================

struct Node {
    string val; // Example: String Concatenation (Non-Commutative)
    Node(string v = "") : val(v) {}
};

const Node NEUTRAL = Node("");

Node merge(const Node& a, const Node& b) {
    if (a.val == "") return b;
    if (b.val == "") return a;
    return Node(a.val + b.val); // Order: A followed by B
}

// =====================================================================
// 2. FIXED ENGINE PART (SEGMENT TREE + HLD ENGINE)
// =====================================================================

struct SegmentTree {
    int n;
    vector<Node> tree, rev_tree; // Normal and reversed interval merges

    SegmentTree(int n = 0) : n(n), tree(4 * n, NEUTRAL), rev_tree(4 * n, NEUTRAL) {}

    void update(int node, int start, int end, int idx, string val) {
        if (start == end) {
            tree[node] = Node(val);
            rev_tree[node] = Node(val);
            return;
        }
        int mid = (start + end) / 2;
        if (idx <= mid) update(2 * node, start, mid, idx, val);
        else update(2 * node + 1, mid + 1, end, idx, val);
        
        tree[node] = merge(tree[2 * node], tree[2 * node + 1]);
        rev_tree[node] = merge(rev_tree[2 * node + 1], rev_tree[2 * node]); // Reversed
    }

    Node query(int node, int start, int end, int l, int r, bool is_reversed = false) {
        if (r < start || end < l) return NEUTRAL;
        if (l <= start && end <= r) return is_reversed ? rev_tree[node] : tree[node];
        int mid = (start + end) / 2;
        if (is_reversed) {
            return merge(query(2 * node + 1, mid + 1, end, l, r, true),
                         query(2 * node, start, mid, l, r, true));
        } else {
            return merge(query(2 * node, start, mid, l, r, false),
                         query(2 * node + 1, mid + 1, end, l, r, false));
        }
    }

    void update(int idx, string val) { update(1, 0, n - 1, idx, val); }
    Node query(int l, int r, bool is_reversed = false) {
        if (l > r) return NEUTRAL;
        return query(1, 0, n - 1, l, r, is_reversed);
    }
};

struct HLD {
    int n, cur_pos;
    vector<vector<int>> adj;
    vector<int> parent, depth, heavy, head, pos, sz;
    vector<string> val;
    SegmentTree seg;

    HLD(int n, const vector<string>& node_vals) : n(n), cur_pos(0), adj(n), parent(n),
        depth(n), heavy(n, -1), head(n), pos(n), sz(n), val(node_vals), seg(n) {}

    void add_edge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    void dfs_sz(int u, int p = -1, int d = 0) {
        sz[u] = 1; depth[u] = d; parent[u] = p;
        int max_c_size = 0;
        for (int v : adj[u]) {
            if (v == p) continue;
            dfs_sz(v, u, d + 1);
            sz[u] += sz[v];
            if (sz[v] > max_c_size) {
                max_c_size = sz[v];
                heavy[u] = v;
            }
        }
    }

    void dfs_hld(int u, int h, int p = -1) {
        head[u] = h;
        pos[u] = cur_pos++;
        seg.update(pos[u], val[u]);

        if (heavy[u] != -1) dfs_hld(heavy[u], h, u);
        for (int v : adj[u]) {
            if (v != p && v != heavy[u]) dfs_hld(v, v, u);
        }
    }

    void init(int root = 0) {
        dfs_sz(root);
        dfs_hld(root, root);
    }

    // Directed Path Query: u -> v preserving exact order
    Node query_path_directed(int u, int v) {
        vector<Node> left_side;  // Path segment: u -> LCA
        vector<Node> right_side; // Path segment: LCA -> v

        while (head[u] != head[v]) {
            if (depth[head[u]] >= depth[head[v]]) {
                // Ascending from u side: query in reversed order (node to head)
                left_side.push_back(seg.query(pos[head[u]], pos[u], true));
                u = parent[head[u]];
            } else {
                // Ascending from v side: query in normal order (head to node)
                right_side.push_back(seg.query(pos[head[v]], pos[v], false));
                v = parent[head[v]];
            }
        }

        if (depth[u] >= depth[v]) {
            left_side.push_back(seg.query(pos[v], pos[u], true));
        } else {
            right_side.push_back(seg.query(pos[u], pos[v], false));
        }

        // Merge left_side in order
        Node res = NEUTRAL;
        for (const auto& node_res : left_side) {
            res = merge(res, node_res);
        }

        // Merge right_side in reverse order
        for (int i = (int)right_side.size() - 1; i >= 0; i--) {
            res = merge(res, right_side[i]);
        }

        return res;
    }
};

// =====================================================================
// 3. MAIN EXECUTION EXAMPLE
// =====================================================================

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n = 5;
    vector<string> node_vals = {"A", "B", "C", "D", "E"};
    HLD hld(n, node_vals);

    hld.add_edge(0, 1);
    hld.add_edge(0, 2);
    hld.add_edge(1, 3);
    hld.add_edge(1, 4);

    hld.init(0);

    // Query string on path 3 -> 2: Node 3 ("D") -> Node 1 ("B") -> Node 0 ("A") -> Node 2 ("C")
    // Expected output: "DBAC"
    cout << "Directed path (3 -> 2): " << hld.query_path_directed(3, 2).val << "\n";

    // Query string on reverse path 2 -> 3: Node 2 ("C") -> Node 0 ("A") -> Node 1 ("B") -> Node 3 ("D")
    // Expected output: "CABD"
    cout << "Directed path (2 -> 3): " << hld.query_path_directed(2, 3).val << "\n";

    return 0;
}
