#include <bits/stdc++.h>
using namespace std;

/*
 * =====================================================================
 * HLD (Heavy-Light Decomposition) Fully Generic Template
 * Complexities:
 *   - Build: O(N)
 *   - Path Query / Update: O(log^2 N)
 *   - Subtree Query / Update: O(log N)
 * Space Complexity: O(N)
 * =====================================================================
 */

// =====================================================================
// 1. DYNAMIC PART (CUSTOMIZE THIS PART FOR EACH PROBLEM)
// =====================================================================

struct Node {
    long long val;
    Node(long long v = 0) : val(v) {}
};

// ---------------------------------------------------------------------
// NEUTRAL IDENTITY ELEMENTS GUIDE:
//   - MAX         : Node(-1e18)
//   - MIN         : Node(1e18)
//   - SUM         : Node(0)
//   - GCD         : Node(0)
//   - BITWISE OR  : Node(0)
//   - BITWISE AND : Node((1LL << 62) - 1)
// ---------------------------------------------------------------------
const long long INF = 1e18;
const Node NEUTRAL = Node(-INF); // CHANGE THIS ACCORDING TO YOUR OPERATION

// ---------------------------------------------------------------------
// MERGE FUNCTION GUIDE:
// Choose or combine operations inside this function.
// ---------------------------------------------------------------------
Node merge(const Node& a, const Node& b) {
    if (a.val == NEUTRAL.val) return b;
    if (b.val == NEUTRAL.val) return a;

    Node res;
    // --- Select desired operation ---
    res.val = max(a.val, b.val);            // MAX
    // res.val = min(a.val, b.val);         // MIN
    // res.val = a.val + b.val;             // SUM
    // res.val = std::gcd(a.val, b.val);    // GCD
    // res.val = a.val | b.val;             // BITWISE OR
    // res.val = a.val & b.val;             // BITWISE AND
    return res;
}

// ---------------------------------------------------------------------
// POINT UPDATE OPERATION GUIDE:
// Specify how point updates behave on a single node.
// ---------------------------------------------------------------------
long long apply_update(long long old_val, long long new_val) {
    return new_val;          // Point Set / Replace Value
    // return old_val + new_val;  // Point Add Value
}


// =====================================================================
// 2. FIXED ENGINE PART (DO NOT TOUCH - HLD + SEGMENT TREE)
// =====================================================================

struct SegmentTree {
    int n;
    vector<Node> tree;

    SegmentTree(int n = 0) : n(n), tree(4 * n, NEUTRAL) {}

    void update(int node, int start, int end, int idx, long long val) {
        if (start == end) {
            tree[node] = Node(apply_update(tree[node].val, val));
            return;
        }
        int mid = (start + end) / 2;
        if (idx <= mid) update(2 * node, start, mid, idx, val);
        else update(2 * node + 1, mid + 1, end, idx, val);
        tree[node] = merge(tree[2 * node], tree[2 * node + 1]);
    }

    Node query(int node, int start, int end, int l, int r) {
        if (r < start || end < l) return NEUTRAL;
        if (l <= start && end <= r) return tree[node];
        int mid = (start + end) / 2;
        return merge(query(2 * node, start, mid, l, r),
                     query(2 * node + 1, mid + 1, end, l, r));
    }

    void update(int idx, long long val) { update(1, 0, n - 1, idx, val); }
    Node query(int l, int r) {
        if (l > r) return NEUTRAL;
        return query(1, 0, n - 1, l, r);
    }
};

struct HLD {
    int n, cur_pos;
    vector<vector<int>> adj;
    vector<int> parent, depth, heavy, head, pos, sz;
    vector<long long> val;
    SegmentTree seg;

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

    // Pass is_edge = true if values belong to EDGES instead of NODES
    Node query_path(int u, int v, bool is_edge = false) {
        Node res = NEUTRAL;
        while (head[u] != head[v]) {
            if (depth[head[u]] < depth[head[v]]) swap(u, v);
            res = merge(res, seg.query(pos[head[u]], pos[u]));
            u = parent[head[u]];
        }
        if (depth[u] > depth[v]) swap(u, v);

        if (is_edge) {
            if (u != v) res = merge(res, seg.query(pos[u] + 1, pos[v])); // Skip LCA for edges
        } else {
            res = merge(res, seg.query(pos[u], pos[v]));
        }
        return res;
    }

    void update_node(int u, long long new_val) {
        val[u] = new_val;
        seg.update(pos[u], new_val);
    }

    Node query_subtree(int u) {
        return seg.query(pos[u], pos[u] + sz[u] - 1);
    }
};


// =====================================================================
// 3. EXAMPLE MAIN FUNCTION
// =====================================================================

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n = 5;
    vector<long long> values = {10, 20, 15, 30, 25}; // Node initial values
    HLD hld(n, values);

    hld.add_edge(0, 1);
    hld.add_edge(0, 2);
    hld.add_edge(1, 3);
    hld.add_edge(1, 4);

    hld.init(0); // Rooted at 0

    // Path Query (Nodes):
    cout << "Path (3 -> 2) Node Max: " << hld.query_path(3, 2).val << "\n";

    // Point Update:
    hld.update_node(2, 50);
    cout << "Path (3 -> 2) Node Max after update: " << hld.query_path(3, 2).val << "\n";

    // Subtree Query:
    cout << "Subtree (1) Max: " << hld.query_subtree(1).val << "\n";

    // Edge-Weighted Query Example: Pass true as 3rd parameter
    // hld.query_path(u, v, true); 

    return 0;
}
