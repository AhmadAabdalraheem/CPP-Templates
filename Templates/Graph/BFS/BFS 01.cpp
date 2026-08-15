// ============================================================================
// 0-1 BFS
// Usage: Shortest path when edge weights are ONLY 0 or 1 (e.g., cell direction changes, 0-cost toggles).
// Key Idea: Weight 0 -> Push to FRONT (high priority). Weight 1 -> Push to BACK.
// Time: O(V + E) [Faster than Dijkstra O((V+E) log V)] | Space: O(V)
// ============================================================================
const int INF = 1e9;
vector<pair<int, int>> adj[N]; // adj[u] = {{neighbor, weight}, ...}
int dist[N];

void bfs01(int start, int n) {
    fill(dist, dist + n + 1, INF); // Initialize distances with Infinity
    deque<int> dq;                 // Deque allows O(1) push to front and back
    
    dist[start] = 0;
    dq.push_back(start);

    while (!dq.empty()) {
        int u = dq.front();
        dq.pop_front();

        for (auto [v, w] : adj[u]) {
            // Relaxation step: found a shorter path to 'v'
            if (dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                
                if (w == 0) {
                    dq.push_front(v); // 0-weight edges processed IMMEDIATELY
                } else {
                    dq.push_back(v);  // 1-weight edges queued normally
                }
            }
        }
    }
}
