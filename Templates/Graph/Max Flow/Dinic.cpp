#include <bits/stdc++.h>
using namespace std;

/*
 * -----------------------------------------------------------------------------
 * TEMPLATE: Dinic's Algorithm (Level Graph + Blocking Flow)
 * -----------------------------------------------------------------------------
 * Time Complexity : O(V^2 * E) 
 * - The fastest and most robust Max Flow algorithm for Competitive Programming.
 * - For Bipartite Matching: O(E * sqrt(V)) -> Matches Hopcroft-Karp speed.
 * * Space Complexity: O(V + E)
 * -----------------------------------------------------------------------------
 */

struct Dinic {
    struct Edge {
        int to;
        long long cap;
        int rev; // Index of the reverse edge in adj[to]
    };

    int n;
    vector<vector<Edge>> adj;
    vector<int> level;
    vector<int> ptr; // Pointer to next unvisited edge (Optimizes DFS)

    // Constructor to initialize the graph with N vertices
    Dinic(int n) : n(n), adj(n), level(n), ptr(n) {}

    // Function to add a directed edge from 'u' to 'v' with capacity 'cap'
    void add_edge(int u, int v, long long cap) {
        adj[u].push_back({v, cap, (int)adj[v].size()});
        adj[v].push_back({u, 0, (int)adj[u].size() - 1}); // Reverse edge starts with 0
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
        return level[t] != -1; // Returns true if sink is reachable
    }

    // DFS to find blocking flow in the level graph
    long long dfs(int u, int t, long long pushed) {
        if (pushed == 0 || u == t) return pushed;
        
        for (int &cid = ptr[u]; cid < adj[u].size(); ++cid) {
            auto &edge = adj[u][cid];
            int v = edge.to;
            
            // Push flow only to the next level
            if (level[u] + 1 != level[v] || edge.cap == 0) continue;
            
            long long tr = dfs(v, t, min(pushed, edge.cap));
            if (tr == 0) continue;
            
            edge.cap -= tr;
            adj[v][edge.rev].cap += tr;
            return tr;
        }
        return 0;
    }

    // Main function to calculate Maximum Flow from source (s) to sink (t)
    long long get_max_flow(int s, int t) {
        long long flow = 0;
        // Keep updating the level graph as long as a path exists
        while (bfs(s, t)) {
            fill(ptr.begin(), ptr.end(), 0); // Reset pointers for DFS
            while (long long pushed = dfs(s, t, 2e18)) { // 2e18 is INF
                flow += pushed;
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

    Dinic flow(n);

    for (int i = 0; i < m; i++) {
        int u, v;
        long long cap;
        cin >> u >> v >> cap;

        // تنبيه الـ Indexing في Codeforces:
        // لو العقد في المسألة تبدأ من 1 (1-indexed)، اطرح 1 كالتالي:
        // flow.add_edge(u - 1, v - 1, cap);
        
        // لو المسألة جاهزة (0-indexed)، استخدمها مباشرة:
        flow.add_edge(u, v, cap);
    }
    
    cout << flow.get_max_flow(source, sink) << "\n";
}

int main() {
    // Fast I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int t = 1;
    // cin >> t; // قم بإلغاء الكومنت لو المسألة تحتوي على Test Cases متعددة
    while (t--) {
        solve();
    }
    return 0;
}
