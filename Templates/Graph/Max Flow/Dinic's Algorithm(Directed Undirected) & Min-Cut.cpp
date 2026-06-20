#include <bits/stdc++.h>
using namespace std;

/*
 * -----------------------------------------------------------------------------
 * TEMPLATE: Dinic's Algorithm with Hybrid (Directed/Undirected) & Min-Cut
 * -----------------------------------------------------------------------------
 * Time Complexity : O(V^2 * E) for Max Flow / Min Cut.
 * Space Complexity: O(V + E)
 * -----------------------------------------------------------------------------
 */

struct Dinic {
    struct Edge {
        int from;
        int to;
        long long cap;
        int rev;          // Index of the reverse edge in adj[to]
        bool is_original; // True if this edge was explicitly added by the user
    };

    int n;
    vector<vector<Edge>> adj;
    vector<int> level;
    vector<int> ptr;
    vector<bool> visited_in_cut;

    // Constructor to initialize the graph with N vertices
    Dinic(int n) : n(n), adj(n), level(n), ptr(n), visited_in_cut(n) {}

    /*
     * add_edge function:
     * - For Directed Graphs  : Set 'undirected = false' (Default). Reverse capacity is 0.
     * - For Undirected Graphs: Set 'undirected = true'. Reverse capacity equals forward capacity.
     */
    void add_edge(int u, int v, long long cap, bool undirected = false) {
        adj[u].push_back({u, v, cap, (int)adj[v].size(), true});
        adj[v].push_back({v, u, (undirected ? cap : 0), (int)adj[u].size() - 1, undirected});
    }

    // BFS to build the level graph
    bool bfs(int s, int t) {
        fill(level.begin(), level.end(), -1);
        level[s] = 0;
        queue<int> q;
        q.push(s);
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            for (auto &edge : adj[u]) {
                if (edge.cap > 0 && level[edge.to] == -1) {
                    level[edge.to] = level[u] + 1;
                    q.push(edge.to);
                }
            }
        }
        return level[t] != -1; // Returns true if the sink is reachable
    }

    // DFS to find blocking flows in the level graph
   long long dfs(int u, int t, long long pushed) {
    if (pushed == 0 || u == t) return pushed;

    long long pushed_total = 0;

    for (int &cid = ptr[u]; cid < adj[u].size(); ++cid) {
        auto &edge = adj[u][cid];
        int v = edge.to;

        if (level[u] + 1 != level[v] || edge.cap == 0) continue;

        long long tr = dfs(v, t, min(pushed, edge.cap));
        if (tr == 0) continue; // Skip if this path can't reach the sink

        // Update residual capacities
        edge.cap -= tr;
        adj[v][edge.rev].cap += tr;

        pushed_total += tr;
        pushed -= tr;

        // If the remaining capacity coming into 'u' is exhausted, we can stop
        if (pushed == 0) break;
    }

    // Optional optimization: If this node couldn't push anything at all,
    // isolate it so future DFS calls in this phase ignore it entirely.
    if (pushed_total == 0) level[u] = -1;

    return pushed_total;
}

    // Main function to calculate Maximum Flow (which equals Min Cut Value)
    long long get_max_flow(int s, int t) {
        long long flow = 0;
        while (bfs(s, t)) {
            fill(ptr.begin(), ptr.end(), 0);
            while (long long pushed = dfs(s, t, 2e18)) { // 2e18 behaves as Infinity
                flow += pushed;
            }
        }
        return flow;
    }

    // Function to extract the actual edges that form the Minimum Cut
    vector<pair<int, int>> get_min_cut_edges(int s) {
        fill(visited_in_cut.begin(), visited_in_cut.end(), false);
        queue<int> q;
        q.push(s);
        visited_in_cut[s] = true;

        // 1. Traverse reachable nodes from source using remaining residual capacity (cap > 0)
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            for (auto &edge : adj[u]) {
                if (edge.cap > 0 && !visited_in_cut[edge.to]) {
                    visited_in_cut[edge.to] = true;
                    q.push(edge.to);
                }
            }
        }

        // 2. Identify cut edges (Original edges going from a visited node to an unvisited node)
        vector<pair<int, int>> cut_edges;
        for (int u = 0; u < n; u++) {
            if (visited_in_cut[u]) {
                for (auto &edge : adj[u]) {
                    if (edge.is_original && !visited_in_cut[edge.to]) {
                        cut_edges.push_back({u, edge.to});
                    }
                }
            }
        }
        return cut_edges;
    }
};

void solve() {
    int n, m;
    if (!(cin >> n >> m)) return;

    int source = 0;
    int sink = n - 1;

    Dinic flow(n);

    for (int i = 0; i < m; i++) {
        int u, v;
        long long cap = 1; // Default capacity for unweighted graph (like Police Chase)

        // If the problem provides edge capacities, uncomment the next line:
        // cin >> u >> v >> cap;
        cin >> u >> v;

        // Pass 'true' if the graph is Undirected, or 'false' if Directed
        flow.add_edge(u - 1, v - 1, cap, true);
    }

    // 1. Calculate Max Flow / Min Cut Value
    long long max_flow = flow.get_max_flow(source, sink);
    cout << max_flow << "\n";

    // 2. Extract and print the actual edges forming the Min Cut
    vector<pair<int, int>> cut = flow.get_min_cut_edges(source);
    for (auto &edge : cut) {
        // Convert back to 1-indexed for the online judge output
        cout << edge.first + 1 << " " << edge.second + 1 << "\n";
    }
}

int main() {
    // Fast I/O for Competitive Programming
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int t = 1;
    // cin >> t; // Uncomment if the problem has multiple test cases
    while (t--) {
        solve();
    }
    return 0;
}
