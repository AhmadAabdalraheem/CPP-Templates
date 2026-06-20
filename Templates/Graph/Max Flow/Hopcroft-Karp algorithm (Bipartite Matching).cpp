#include <bits/stdc++.h>
using namespace std;

/*
 * -----------------------------------------------------------------------------
 * TEMPLATE: Hopcroft-Karp Algorithm (Maximum Bipartite Matching)
 * -----------------------------------------------------------------------------
 * Time Complexity : O(E * sqrt(V)) 
 * - The fastest and most efficient matching algorithm for Competitive Programming.
 * - Drastically lower constant factor compared to standard max flow algorithms.
 * Space Complexity: O(V + E)
 * -----------------------------------------------------------------------------
 */

struct HopcroftKarp {
    const int INF = 1e9;
    int n, m; // n: size of Left set, m: size of Right set
    vector<vector<int>> adj;
    vector<int> match_l, match_r, dist;

    // Constructor to initialize with the size of Left (n) and Right (m) sets
    HopcroftKarp(int n, int m) : n(n), m(m), adj(n), match_l(n, -1), match_r(m, -1), dist(n) {}

    // Function to add a directed edge from Left set (u) to Right set (v)
    void add_edge(int u, int v) {
        adj[u].push_back(v);
    }

    // BFS to find augmenting paths and layer the graph
    bool bfs() {
        queue<int> q;
        for (int u = 0; u < n; ++u) {
            if (match_l[u] == -1) {
                dist[u] = 0;
                q.push(u);
            } else {
                dist[u] = INF;
            }
        }
        
        int min_dist = INF;
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            
            if (dist[u] >= min_dist) continue;
            
            for (int v : adj[u]) {
                if (match_r[v] == -1) {
                    min_dist = min(min_dist, dist[u] + 1);
                } else if (dist[match_r[v]] == INF) {
                    dist[match_r[v]] = dist[u] + 1;
                    q.push(match_r[v]);
                }
            }
        }
        return min_dist != INF; // Returns true if an augmenting path is found
    }

    // DFS to match vertices along the augmenting paths
    bool dfs(int u) {
        for (int v : adj[u]) {
            if (match_r[v] == -1 || (dist[match_r[v]] == dist[u] + 1 && dfs(match_r[v]))) {
                match_l[u] = v;
                match_r[v] = u;
                return true;
            }
        }
        dist[u] = INF;
        return false;
    }

    // Main function to calculate the Maximum Bipartite Matching
    int get_max_matching() {
        int matching = 0;
        while (bfs()) {
            for (int u = 0; u < n; ++u) {
                if (match_l[u] == -1 && dfs(u)) {
                    matching++;
                }
            }
        }
        return matching;
    }
};

void solve(){
    int n, m, edges;
    if (!(cin >> n >> m >> edges)) return;
    
    // n: عدد عناصر المجموعة اليسرى (Left Set)
    // m: عدد عناصر المجموعة اليمنى (Right Set)
    HopcroftKarp hk(n, m);

    for (int i = 0; i < edges; i++) {
        int u, v;
        cin >> u >> v;

        // تنبيه الـ Indexing في Codeforces:
        // لو المجموعات في المسألة تبدأ من 1 (1-indexed)، اطرح 1 كالتالي:
        // hk.add_edge(u - 1, v - 1);
        
        // لو المسألة جاهزة (0-indexed)، استخدمها مباشرة:
        hk.add_edge(u, v);
    }
    
    cout << hk.get_max_matching() << "\n";

    // لو مطلوب منك طباعة أزواج الـ matching المستخرجة:
    // for (int u = 0; u < n; ++u) {
    //     if (hk.match_l[u] != -1) {
    //         cout << u << " matched with " << hk.match_l[u] << "\n";
    //     }
    // }
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
