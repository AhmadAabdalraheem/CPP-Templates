#include <bits/stdc++.h>
using namespace std;

/*
 * =====================================================================
 * PATTERN 2: RANGE PATH & SUBTREE UPDATES (HLD + LAZY PROPAGATION)
 * =====================================================================
 * 
 * 📌 WHEN TO USE THIS TEMPLATE:
 * ---------------------------------------------------------------------
 * 1. The problem asks for RANGE UPDATES on a path (u -> v) or a Subtree.
 *    Examples: "Add X to all nodes on path u -> v", "Set all nodes in 
 *    u's subtree to value X".
 * 2. Combined with path or subtree range queries (Sum, Min, Max, etc.).
 * 
 * 💡 CORE IDEA & IMPLEMENTATION DETAILS:
 * ---------------------------------------------------------------------
 * - Replaces the point-update Segment Tree with a Lazy Propagation Segment Tree.
 * - Path Update (`update_path`): Iterates over heavy chains and applies 
 *   range updates `[pos[head[u]], pos[u]]` in O(log^2 N).
 * - Subtree Update (`update_subtree`): Since HLD maps a subtree to a contiguous 
 *   range `[pos[u], pos[u] + sz[u] - 1]`, a subtree update takes only O(log N).
 * 
 * ⏱️ COMPLEXITIES:
 * ---------------------------------------------------------------------
 * - Build HLD: O(N)
 * - Path Query / Update: O(log^2 N)
 * - Subtree Query / Update: O(log N)
 * - Space Complexity: O(N)
 * =====================================================================
 */

// =====================================================================
// 1. DYNAMIC PART (CUSTOMIZE FOR YOUR OPERATION)
// =====================================================================

struct Node {
    long long val;
    Node(long long v = 0) : val(v) {}
};

const long long INF = 1e18;
const Node NEUTRAL = Node(0); // Neutral element for SUM

Node merge(const Node& a, const Node& b) {
    if (a.val == NEUTRAL.val && &a == &NEUTRAL) return b;
    if (b.val == NEUTRAL.val && &b == &NEUTRAL) return a;
    return Node(a.val + b.val); // Operation: SUM
}

// =====================================================================
// 2. FIXED ENGINE PART (LAZY SEGMENT TREE + HLD ENGINE)
// =====================================================================

struct LazySegmentTree {
    int n;
    vector<Node> tree;
    vector<long long> lazy;

    LazySegmentTree(int n = 0) : n(n), tree(4 * n, NEUTRAL), lazy(4 * n, 0) {}

    void push(int node, int start, int end) {
        if (lazy[node] == 0) return;
        
        // Apply pending lazy operation (Range Addition for SUM query)
        tree[node].val += lazy[node] * (end - start + 1);

        if (start != end) {
            lazy[2 * node] += lazy[node];
            lazy[2 * node + 1] += lazy[node];
        }
        lazy[node] = 0;
    }

    void update_range(int node, int start, int end, int l, int r, long long val) {
        push(node, start, end);
        if (r < start || end < l) return;
        if (l <= start && end <= r) {
            lazy[node] += val;
            push(node, start, end);
            return;
        }
        int mid = (start + end) / 2;
        update_range(2 * node, start, mid, l, r, val);
        update_range(2 * node + 1, mid + 1, end, l, r, val);
        tree[node] = merge(tree[2 * node], tree[2 * node + 1]);
    }

    Node query_range(int node, int start, int end, int l, int r) {
        push(node, start, end);
        if (r < start || end < l) return NEUTRAL;
        if (l <= start && end <= r) return tree[node];
        int mid = (start + end) / 2;
        return merge(query_range(2 * node, start, mid, l, r),
                     query_range(2 * node + 1, mid + 1, end, l, r));
    }

    void update_range(int l, int r, long long val) {
        if (l > r) return;
        update_range(1, 0, n - 1, l, r, val);
    }

    Node query_range(int l, int r) {
        if (l > r) return NEUTRAL;
        return query_range(1, 0, n - 1, l, r);
    }
};

struct HLD {
    int n, cur_pos;
    vector<vector<int>> adj;
    vector<int> parent, depth, heavy, head, pos, sz;
    vector<long long> val;
    LazySegmentTree seg;

    HLD(int n, const vector<long long>& node_vals) : n(n), cur_pos(0), adj(n), parent(n),
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
        seg.update_range(pos[u], pos[u], val[u]);

        if (heavy[u] != -1) dfs_hld(heavy[u], h, u);
        for (int v : adj[u]) {
            if (v != p && v != heavy[u]) dfs_hld(v, v, u);
        }
    }

    void init(int root = 0) {
        dfs_sz(root);
        dfs_hld(root, root);
    }

    // Path Range Update: Add val to all nodes on path u -> v
    void update_path(int u, int v, long long add_val) {
        while (head[u] != head[v]) {
            if (depth[head[u]] < depth[head[v]]) swap(u, v);
            seg.update_range(pos[head[u]], pos[u], add_val);
            u = parent[head[u]];
        }
        if (depth[u] > depth[v]) swap(u, v);
        seg.update_range(pos[u], pos[v], add_val);
    }

    // Path Range Query: Query sum on path u -> v
    Node query_path(int u, int v) {
        Node res = NEUTRAL;
        while (head[u] != head[v]) {
            if (depth[head[u]] < depth[head[v]]) swap(u, v);
            res = merge(res, seg.query_range(pos[head[u]], pos[u]));
            u = parent[head[u]];
        }
        if (depth[u] > depth[v]) swap(u, v);
        res = merge(res, seg.query_range(pos[u], pos[v]));
        return res;
    }

    // Subtree Range Update: Add val to all nodes in u's subtree
    void update_subtree(int u, long long add_val) {
        seg.update_range(pos[u], pos[u] + sz[u] - 1, add_val);
    }

    // Subtree Range Query: Query sum in u's subtree
    Node query_subtree(int u) {
        return seg.query_range(pos[u], pos[u] + sz[u] - 1);
    }
};

// =====================================================================
// 3. MAIN EXECUTION EXAMPLE
// =====================================================================

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n = 5;
    vector<long long> node_vals = {1, 2, 3, 4, 5};
    HLD hld(n, node_vals);

    hld.add_edge(0, 1);
    hld.add_edge(0, 2);
    hld.add_edge(1, 3);
    hld.add_edge(1, 4);

    hld.init(0);

    // Initial Path Sum (3 -> 2): Node 3 (4) + Node 1 (2) + Node 0 (1) + Node 2 (3) = 10
    cout << "Path sum (3 -> 2): " << hld.query_path(3, 2).val << "\n";

    // Range Add +10 to path (3 -> 2)
    hld.update_path(3, 2, 10);
    cout << "Path sum (3 -> 2) after range add: " << hld.query_path(3, 2).val << "\n";

    // Subtree Range Add +5 to node 1's subtree
    hld.update_subtree(1, 5);
    cout << "Subtree sum (1) after range add: " << hld.query_subtree(1).val << "\n";

    return 0;
}
