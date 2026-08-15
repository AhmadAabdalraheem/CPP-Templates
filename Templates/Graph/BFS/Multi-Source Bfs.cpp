// ============================================================================
// MULTI-SOURCE BFS
// Usage: Distance to NEAREST special node among multiple (e.g., distance to nearest fire/monster/exit).
// Key Idea: Initialize Queue with ALL sources at distance 0 simultaneously.
// Time: O(V + E) | Space: O(V)
// ============================================================================
void multiSourceBFS(const vector<int>& sources, int n) {
    fill(dist, dist + n + 1, -1);
    queue<int> q;

    // Insert ALL starting points into the queue at distance 0
    for (int src : sources) {
        dist[src] = 0;
        q.push(src);
    }

    // Standard BFS expands radially from ALL sources in parallel
    while (!q.empty()) {
        int u = q.front();
        q.pop();

        for (int v : adj[u]) {
            if (dist[v] == -1) {
                dist[v] = dist[u] + 1;
                q.push(v);
            }
        }
    }
}
