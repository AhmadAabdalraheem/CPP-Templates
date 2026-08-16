#include <bits/stdc++.h>
using namespace std;

/*
 * =====================================================================
 * PATTERN 4: DYNAMIC ROOTING SUBTREE QUERIES
 * =====================================================================
 * 
 * 📌 WHEN TO USE THIS TEMPLATE:
 * ---------------------------------------------------------------------
 * 1. The problem includes queries that change the tree's root dynamically: 
 *    `Reroot(R)`.
 * 2. You need to handle SUBTREE QUERIES under the new root `R` WITHOUT 
 *    rebuilding the HLD tree.
 * 
 * 💡 CORE IDEA & IMPLEMENTATION DETAILS:
 * ---------------------------------------------------------------------
 * - PATH queries NEVER change when the root changes! Path u -> v is invariant.
 * - SUBTREE queries change based on the relative position of node `u` and root `R`:
 *     Case 1: `u == R` -> The subtree is the ENTIRE tree. Query range `[0, N - 1]`.
 *     Case 2: `R` is NOT in `u`'s subtree -> The subtree is UNCHANGED. Query normal range.
 *     Case 3: `R` IS in `u`'s subtree -> Find the child `c` of `u` that is an 
 *             ancestor of `R`. The subtree of `u` under root `R` is the ENTIRE tree 
 *             EXCEPT the subtree of `c`. Query `[0, pos[c]-1] ∪ [pos[c]+sz[c], N-1]`.
 * 
 * ⏱️ COMPLEXITIES:
 * ---------------------------------------------------------------------
 * - Build HLD: O(N)
 * - Dynamic Root Subtree Query: O(log N)
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
const Node NEUTRAL = Node(-INF);

Node merge(const Node& a, const Node& b) {
    if (a.val == NEUTRAL.val) return b;
    if (b.val == NEUTRAL.val) return a;
    return Node(max(a.val, b.val)); // MAX
}

// =====================================================================
// 2. FIXED ENGINE PART (SEGMENT TREE + HLD ENGINE)
// =====================================================================

struct SegmentTree {
    int n;
    vector<Node> tree;

    SegmentTree(int n = 0) : n(n), tree(4 * n, NEUTRAL) {}

    void update(int node, int start, int end, int idx, long long val) {
        if (start == end) {
            tree[node] = Node(val);
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

    // Helper: Check if 'u' is ancestor of 'v' under default root
    bool is_ancestor(int u, int v) {
        return pos[u] <= pos[v] && pos[v] < pos[u] + sz[u];
    }

    // Helper: Find child of 'u' that is ancestor of 'v'
    int get_child_towards(int u, int v) {
        while (head[v] != head[u]) {
            if (parent[head[v]] == u) return head[v];
            v = parent[head[v]];
        }
        return heavy[u];
    }

    // Query Subtree of node 'u' when current root is 'r'
    Node query_rerooted_subtree(int u, int r) {
        // Case 1: u is the current root -> Entire tree
        if (u == r) {
            return seg.query(0, n - 1);
        }

        // Case 2: Root 'r' is NOT inside subtree of 'u' -> Normal subtree query
        if (!is_ancestor(u, r)) {
            return seg.query(pos[u], pos[u] + sz[u] - 1);
        }

        // Case 3: Root 'r' IS inside subtree of 'u' -> Everything EXCEPT child's subtree
        int child = get_child_towards(u, r);
        Node left_part = seg.query(0, pos[child] - 1);
        Node right_part = seg.query(pos[child] + sz[child], n - 1);

        return merge(left_part, right_part);
    }
};

// =====================================================================
// 3. MAIN EXECUTION EXAMPLE
// =====================================================================

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n = 5;
    vector<long long> node_vals = {10, 20, 15, 30, 25};
    HLD hld(n, node_vals);

    hld.add_edge(0, 1);
    hld.add_edge(0, 2);
    hld.add_edge(1, 3);
    hld.add_edge(1, 4);

    hld.init(0); // Default root = 0

    int current_root = 0;

    // Subtree max of node 1 under default root (0): Nodes 1, 3, 4 -> Max = 30
    cout << "Subtree (1) Max [Root = 0]: " << hld.query_rerooted_subtree(1, current_root).val << "\n";

    // Change root to node 3
    current_root = 3;

    // Subtree max of node 1 under new root (3): Entire tree EXCEPT node 3 -> Max = 25
    cout << "Subtree (1) Max [Root = 3]: " << hld.query_rerooted_subtree(1, current_root).val << "\n";

    return 0;
}
