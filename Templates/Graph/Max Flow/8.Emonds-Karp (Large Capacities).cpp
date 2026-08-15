#include <bits/stdc++.h>
using namespace std;

/*
 * -----------------------------------------------------------------------------
 * TEMPLATE: Edmonds-Karp Algorithm (BFS-based Max Flow)
 * -----------------------------------------------------------------------------
 * Time Complexity : O(V * E^2) 
 * where 'V' is vertices and 'E' is edges.
 * - SAFE from large capacities (e.g., 10^9) because it doesn't depend on flow value.
 * - Best when V <= 1000 and E <= 5000.
 * * Space Complexity: O(V + E)
 * -----------------------------------------------------------------------------
 */

struct EdmondsKarp {
    struct Edge {
        int to;
        long long cap;
        int rev; // Index of the reverse edge in adj[to]
    };

    int n;
    vector<vector<Edge>> adj;
    vector<int> parent_node;
    vector<int> parent_edge;

    // Constructor to initialize the graph with N vertices
    EdmondsKarp(int n) : n(n), adj(n), parent_node(n), parent_edge(n) {}

    // Function to add a directed edge from 'u' to 'v' with capacity 'cap'
    void add_edge(int u, int v, long long cap) {
        adj[u].push_back({v, cap, (int)adj[v].size()});
        adj[v].push_back({u, 0, (int)adj[u].size() - 1}); // Reverse edge starts with 0 capacity
    }

    // BFS to find the shortest augmenting path
    long long bfs(int s, int t) {
        fill(parent_node.begin(), parent_node.end(), -1);
        parent_node[s] = s;
        
        queue<pair<int, long long>> q; // {current_node, current_bottleneck}
        q.push({s, 2e18}); // oo (infinity)

        while (!q.empty()) {
            int u = q.front().first;
            long long flow = q.front().second;
            q.pop();

            for (int i = 0; i < adj[u].size(); i++) {
                auto &edge = adj[u][i];
                if (parent_node[edge.to] == -1 && edge.cap > 0) {
                    parent_node[edge.to] = u;
                    parent_edge[edge.to] = i;
                    long long new_flow = min(flow, edge.cap);
                    
                    if (edge.to == t) return new_flow; // Reached the sink
                    q.push({edge.to, new_flow});
                }
            }
        }
        return 0; // No more paths
    }

    // Main function to calculate Maximum Flow from source (s) to sink (t)
    long long get_max_flow(int s, int t) {
        long long flow = 0;
        while (true) {
            long long pushed = bfs(s, t);
            if (pushed == 0) break; // If no path is found, algorithm terminates

            flow += pushed;
            
            // Augment flow along the path found by BFS
            int curr = t;
            while (curr != s) {
                int p = parent_node[curr];
                int idx = parent_edge[curr];
                
                adj[p][idx].cap -= pushed;
                int rev_idx = adj[p][idx].rev;
                adj[curr][rev_idx].cap += pushed;
                
                curr = p;
            }
        }
        return flow;
    }
};

void solve(){
    int n, m;
    if (!(cin >> n >> m)) return;
    
    // حدد الـ source والـ sink بناءً على معطيات المسألة
    int source = 0; 
    int sink = n - 1;

    EdmondsKarp flow(n);

    for (int i = 0; i < m; i++) {
        int u, v;
        long long cap;
        cin >> u >> v >> cap;

        // لو العقد في المسألة تبدأ من 1 (1-indexed)، اطرح 1 كالتالي:
        // flow.add_edge(u - 1, v - 1, cap);
        
        // لو المسألة جاهزة (0-indexed)، استخدمها مباشرة:
        flow.add_edge(u, v, cap);
    }
    
    cout << flow.get_max_flow(source, sink) << "\n";
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int t = 1;
    while (t--) {
        solve();
    }
    return 0;
}
