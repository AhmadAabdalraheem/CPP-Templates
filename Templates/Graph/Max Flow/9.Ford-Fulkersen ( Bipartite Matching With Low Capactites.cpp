#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
const ll INF = 1e18;

/*
 * -----------------------------------------------------------------------------
 * TEMPLATE: Ford-Fulkerson Algorithm (DFS-based)
 * -----------------------------------------------------------------------------
 * Time Complexity : O(E * f) 
 * where 'E' is the number of edges and 'f' is the maximum flow.
 * - Excellent for Bipartite Matching: O(E * V)
 * - DANGEROUS if capacities are large (e.g., 10^9) -> Can cause TLE.
 * * Space Complexity: O(V + E) -> Uses Adjacency List, very memory efficient.
 * -----------------------------------------------------------------------------
 */

struct FordFulkerson {
    struct Edge {
        int to;
        ll cap;
        int rev; // Index of the reverse edge in adj[to]
    };

    int n;
    vector<vector<Edge>> adj;
    vector<bool> visited;

    // Constructor to initialize the graph with N vertices
    FordFulkerson(int n) : n(n), adj(n), visited(n) {}

    // Function to add a directed edge from 'u' to 'v' with capacity 'cap'
    void add_edge(int u, int v, ll cap) {
        adj[u].push_back({v, cap, (int)adj[v].size()});
        adj[v].push_back({u, 0, (int)adj[u].size() - 1}); // Reverse edge starts with 0 capacity
    }

    // DFS to find an augmenting path
    ll dfs(int u, int t, ll push) {
        if (u == t) return push;
        visited[u] = true;

        for (auto &edge : adj[u]) {
            if (!visited[edge.to] && edge.cap > 0) {
                ll tr = dfs(edge.to, t, min(push, edge.cap));
                
                // If a valid path to the sink is found
                if (tr > 0) {
                    edge.cap -= tr;                    // Decrease forward capacity
                    adj[edge.to][edge.rev].cap += tr; // Increase backward capacity
                    return tr;
                }
            }
        }
        return 0;
    }

    // Main function to calculate Maximum Flow from source (s) to sink (t)
    ll get_max_flow(int s, int t) {
        ll flow = 0;
        while (true) {
            fill(visited.begin(), visited.end(), false);
            ll pushed = dfs(s, t, INF);
            
            if (pushed == 0) break; // No more augmenting paths found
            flow += pushed;
        }
        return flow;
    }
};

void solve() {
    int vertices, edges;
    cin >> vertices >> edges;

    // Setup source and sink (0-indexed)
    int source = 0;
    int sink = vertices - 1;

    FordFulkerson solver(vertices);

    for (int i = 0; i < edges; i++) {
        int u, v;
        ll cap;
        cin >> u >> v >> cap;
        
        // Note: If the problem is 1-indexed, use: solver.add_edge(u-1, v-1, cap);
        solver.add_edge(u, v, cap);
    }

    cout << solver.get_max_flow(source, sink) << "\n";
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
