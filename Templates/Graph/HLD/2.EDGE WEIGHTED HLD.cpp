#include <bits/stdc++.h>
using namespace std;

/*
 * =====================================================================
 * PATTERN 1: EDGE-WEIGHTED HLD TEMPLATE (FULL & SELF-CONTAINED)
 * =====================================================================
 * 
 * 📌 WHEN TO USE THIS TEMPLATE:
 * ---------------------------------------------------------------------
 * 1. The problem provides a tree with weights on EDGES instead of NODES.
 * 2. You need to process path queries between nodes (u, v) such as:
 *    - Max / Min edge weight on path.
 *    - Sum of edge weights on path.
 *    - Greatest Common Divisor (GCD) or Bitwise operations on edges.
 * 3. Point updates on specific edge weights given by their Edge ID.
 * 
 * ⏱️ COMPLEXITIES:
 * ---------------------------------------------------------------------
 * - Build HLD: O(N)
 * - Path Query / Edge Update: O(log^2 N)
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

// Neutral Identity Elements:
//   - MAX : -INF (-1e18) | MIN : +INF (+1e18) | SUM : 0 | GCD : 0
const long long INF = 1e18;
const Node NEUTRAL = Node(-INF); 

Node merge(const Node& a, const Node& b) {
    if (a.val == NEUTRAL.val) return b;
    if (b.val == NEUTRAL.val) return a;
    
    // --- Select desired operation ---
    return Node(max(a.val, b.val)); // MAX
}

long long apply_update(long long old_val, long long new_val) {
    return new_val; // Point Set / Value Replacement
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

struct Edge {
    int u, v;
    long long w;
    int child_node; // Node holding this edge's weight
};

struct HLD {
    int n, cur_pos;
    vector<vector<int>> adj;
    vector<int> parent, depth, heavy, head, pos, sz;
    vector<long long> node_val;
    SegmentTree seg;
    vector<Edge> edges; // 1-indexed original edges

    HLD(int n, int num_edges) : n(n), cur_pos(0), adj(n), parent(n),
        depth(n), heavy(n, -1), head(n), pos(n), sz(n), node_val(n, NEUTRAL.val), seg(n), edges(num_edges + 1) {}

    void add_edge(int edge_id, int u, int v, long long w) {
        edges[edge_id] = {u, v, w, -1};
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
        seg.update(pos[u], node_val[u]);

        if (heavy[u] != -1) dfs_hld(heavy[u], h, u);
        for (int v : adj[u]) {
            if (v != p && v != heavy[u]) dfs_hld(v, v, u);
        }
    }

    void init(int root = 0) {
        dfs_sz(root);
        
        // Map edge weights to the deeper child node
        for (size_t i = 1; i < edges.size(); i++) {
            auto& e = edges[i];
            if (depth[e.u] < depth[e.v]) swap(e.u, e.v);
            e.child_node = e.u;
            node_val[e.child_node] = e.w;
        }

        dfs_hld(root, root);
    }

    // Path Query (Automatically skips LCA node)
    Node query_edge_path(int u, int v) {
        Node res = NEUTRAL;
        while (head[u] != head[v]) {
            if (depth[head[u]] < depth[head[v]]) swap(u, v);
            res = merge(res, seg.query(pos[head[u]], pos[u]));
            u = parent[head[u]];
        }
        if (depth[u] > depth[v]) swap(u, v);

        // Skip LCA node (pos[u] + 1)
        if (u != v) {
            res = merge(res, seg.query(pos[u] + 1, pos[v]));
        }
        return res;
    }

    // Update weight of the i-th edge
    void update_edge(int edge_id, long long new_w) {
        int target_node = edges[edge_id].child_node;
        node_val[target_node] = new_w;
        seg.update(pos[target_node], new_w);
    }
};


// =====================================================================
// 3. MAIN EXECUTION EXAMPLE
// =====================================================================

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n = 5;
    HLD hld(n, n - 1); // 5 nodes, 4 edges

    // add_edge(edge_id, u, v, weight)
    hld.add_edge(1, 0, 1, 10);
    hld.add_edge(2, 0, 2, 20);
    hld.add_edge(3, 1, 3, 30);
    hld.add_edge(4, 1, 4, 25);

    hld.init(0);

    // Query Path (3 -> 2)
    cout << "Max edge weight (3 -> 2): " << hld.query_edge_path(3, 2).val << "\n";

    // Update Edge 3 to 5
    hld.update_edge(3, 5);
    cout << "Max edge weight (3 -> 2) after update: " << hld.query_edge_path(3, 2).val << "\n";

    return 0;
}
