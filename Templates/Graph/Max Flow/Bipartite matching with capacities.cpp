#include <bits/stdc++.h>
using namespace std;

/*
 * -----------------------------------------------------------------------------
 * TEMPLATE: Dinic's Algorithm (Max Flow with Custom Capacities)
 * -----------------------------------------------------------------------------
 * Time Complexity : O(V^2 * E) worst case, but on Bipartite/Unit networks it runs 
 * in O(E * sqrt(V)), matching Hopcroft-Karp's speed!
 * Space Complexity: O(V + E)
 * -----------------------------------------------------------------------------
 */

struct Edge {
    int to;
    int flow;
    int cap;
    int rev; // Index of the reverse edge in the adjacency list of 'to'
};

struct Dinic {
    const int INF = 1e9;
    int n;
    vector<vector<Edge>> adj;
    vector<int> level;
    vector<int> ptr;

    // Constructor: Initialize with the total number of nodes in the network
    Dinic(int n) : n(n), adj(n), level(n), ptr(n) {}

    // Function to add a directed edge with a specific capacity
    void add_edge(from, int to, int cap) {
        adj[from].push_back({to, 0, cap, (int)adj[to].size()});
        adj[to].push_back({from, 0, 0, (int)adj[from].size() - 1}); // Reverse edge initially has 0 capacity
    }

    // BFS to find augmenting paths and build the level graph
    bool bfs(int s, int t) {
        fill(level.begin(), level.end(), -1);
        level[s] = 0;
        queue<int> q;
        q.push(s);
        while (!q.empty()) {
            int v = q.front();
            q.pop();
            for (auto& edge : adj[v]) {
                if (edge.cap - edge.flow > 0 && level[edge.to] == -1) {
                    level[edge.to] = level[v] + 1;
                    q.push(edge.to);
                }
            }
        }
        return level[t] != -1; // Returns true if the sink is reachable
    }

    // DFS to send flow along the shortest augmenting paths
    int dfs(int v, int t, int pushed) {
        if (pushed == 0) return 0;
        if (v == t) return pushed;
        for (int& cid = ptr[v]; cid < adj[v].size(); ++cid) {
            auto& edge = adj[v][cid];
            int tr = edge.to;
            if (level[v] + 1 != level[tr] || edge.cap - edge.flow == 0) continue;
            int tr_pushed = dfs(tr, t, min(pushed, edge.cap - edge.flow));
            if (tr_pushed == 0) continue;
            edge.flow += tr_pushed;
            adj[tr][edge.rev].flow -= tr_pushed;
            return tr_pushed;
        }
        return 0;
    }

    // Main function to calculate the Maximum Flow from Source (s) to Sink (t)
    int get_max_flow(int s, int t) {
        int flow = 0;
        while (bfs(s, t)) {
            fill(ptr.begin(), ptr.end(), 0);
            while (int pushed = dfs(s, t, INF)) {
                flow += pushed;
            }
        }
        return flow;
    }
};

void solve(){
    int n, m, edges;
    if (!(cin >> n >> m >> edges)) return;

    // n: Number of elements in the Left Set (G1)
    // m: Number of elements in the Right Set (G2)
    
    // Network Node Indexing Mapping:
    // Source (s) = 0
    // Group 1 (G1) nodes: 1 to n
    // Group 2 (G2) nodes: n + 1 to n + m
    // Sink (t) = n + m + 1
    
    int total_nodes = n + m + 2;
    int s = 0;
    int t = n + m + 1;

    Dinic dinic(total_nodes);

    // 1. Set custom capacities between Source (s) and Group 1 (G1)
    for (int i = 1; i <= n; i++) {
        int cap_s_to_g1 = 1; // Change this value to scale capacities for G1 nodes
        dinic.add_edge(s, i, cap_s_to_g1);
    }

    // 2. Read edges between Group 1 (G1) and Group 2 (G2)
    for (int i = 0; i < edges; i++) {
        int u, v;
        cin >> u >> v;
        
        // Codeforces Indexing Note:
        // If the problem is 1-indexed: u is ready for G1, and we offset v by adding n
        // If the problem is 0-indexed: use (u + 1) and (v + 1 + n)
        int g1_node = u; 
        int g2_node = v + n; 
        
        int edge_cap = 1; // Capacity between G1 and G2 nodes (usually 1, or custom)
        dinic.add_edge(g1_node, g2_node, edge_cap);
    }

    // 3. Set custom capacities between Group 2 (G2) and Sink (t)
    for (int j = 1; j <= m; j++) {
        int g2_node = n + j;
        int cap_g2_to_t = 1; // Change this value to scale capacities for G2 nodes
        dinic.add_edge(g2_node, t, cap_g2_to_t);
    }

    // Output the Maximum Flow (which corresponds to the maximum matching with custom capacities)
    cout << dinic.get_max_flow(s, t) << "\n";

    // Optional: Print the matched pairs between G1 and G2
    for (int u = 1; u <= n; u++) {
        for (auto& edge : dinic.adj[u]) {
            // Check if the edge goes to G2 (nodes numbered from n + 1 to n + m) 
            // and has positive flow (meaning it is selected)
            if (edge.to > n && edge.to <= n + m && edge.flow > 0) {
                int g1_node = u;
                int g2_node = edge.to - n; // Subtract n to get the original index in G2
                
                cout << g1_node << " matched with " << g2_node;
                
                // If capacity can be greater than 1, you can also print how much flow passed
                // cout << " (Flow: " << edge.flow << ")"; 
                cout << "\n";
            }
        }
    }
}

int main() {
    // Fast I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int t = 1;
    // cin >> t; // Uncomment if the problem has multiple test cases
    while (t--) {
        solve();
    }
    return 0;
}
