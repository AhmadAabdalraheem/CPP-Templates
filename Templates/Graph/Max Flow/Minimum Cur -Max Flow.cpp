#include <bits/stdc++.h>
using namespace std;

/*
 * -----------------------------------------------------------------------------
 * TEMPLATE: Dinic's Algorithm with Min-Cut Extraction
 * -----------------------------------------------------------------------------
 * Time Complexity : O(V^2 * E) for Max Flow.
 * - Extra O(V + E) to extract the Min-Cut edges using a final BFS.
 * Space Complexity: O(V + E)
 * -----------------------------------------------------------------------------
 */

struct DinicWithMinCut {
    struct Edge {
        int from;
        int to;
        long long cap;
        long long original_cap; // Needed to track original edges during the cut extraction
        int rev;
    };

    int n;
    vector<vector<Edge>> adj;
    vector<int> level;
    vector<int> ptr;
    vector<bool> visited_in_cut; // To mark nodes belonging to the Source (S) side of the cut

    DinicWithMinCut(int n) : n(n), adj(n), level(n), ptr(n), visited_in_cut(n) {}

    // Add a directed edge from 'u' to 'v' with capacity 'cap'
    void add_edge(int u, int v, long long cap) {
        adj[u].push_back({u, v, cap, cap, (int)adj[v].size()});
        adj[v].push_back({v, u, 0, 0, (int)adj[u].size() - 1}); // Residual reverse edge starts with 0
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
        for (int &cid = ptr[u]; cid < adj[u].size(); ++cid) {
            auto &edge = adj[u][cid];
            int v = edge.to;
            if (level[u] + 1 != level[v] || edge.cap == 0) continue;
            long long tr = dfs(v, t, min(pushed, edge.cap));
            if (tr == 0) continue;
            edge.cap -= tr;
            adj[v][edge.rev].cap += tr;
            return tr;
        }
        return 0;
    }

    // Main function to calculate Maximum Flow
    long long get_max_flow(int s, int t) {
        long long flow = 0;
        while (bfs(s, t)) {
            fill(ptr.begin(), ptr.end(), 0);
            while (long long pushed = dfs(s, t, 2e18)) {
                flow += pushed;
            }
        }
        return flow;
    }

    // Function to extract the edges that form the Minimum Cut
    vector<pair<int, int>> get_min_cut_edges(int s) {
        fill(visited_in_cut.begin(), visited_in_cut.end(), false);
        
        // 1. BFS from source traversing only edges with remaining residual capacity (cap > 0)
        queue<int> q;
        q.push(s);
        visited_in_cut[s] = true;

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

        // 2. Identify cut edges: original edges going from a visited node to an unvisited node
        vector<pair<int, int>> cut_edges;
        for (int u = 0; u < n; u++) {
            if (visited_in_cut[u]) {
                for (auto &edge : adj[u]) {
                    // Ensure it's an original network edge and goes to the unvisited side (T)
                    if (edge.original_cap > 0 && !visited_in_cut[edge.to]) {
                        cut_edges.push_back({u, edge.to});
                    }
                }
            }
        }
        return cut_edges;
    }
};

void solve(){
    int n, m;
    if (!(cin >> n >> m)) return;

    int source = 0;
    int sink = n - 1;

    DinicWithMinCut flow(n);

    for (int i = 0; i < m; i++) {
        int u, v;
        long long cap;
        cin >> u >> v >> cap;
        // Note: If the problem is 1-indexed, use: flow.add_edge(u-1, v-1, cap);
        flow.add_edge(u, v, cap);
    }

    // 1. Calculate Max Flow (which equals Min Cut Value)
    long long max_flow = flow.get_max_flow(source, sink);
    cout << "Max Flow / Min Cut Value: " << max_flow << "\n";

    // 2. Extract and print the actual edges that need to be cut
    vector<pair<int, int>> cut = flow.get_min_cut_edges(source);
    
    cout << "Edges to cut:\n";
    for (auto &edge : cut) {
        cout << edge.first << " -> " << edge.second << "\n";
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int t = 1;
    while (t--) {
        solve();
    }
