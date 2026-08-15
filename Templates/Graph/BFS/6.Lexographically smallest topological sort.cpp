// ============================================================================
// LEXICOGRAPHICALLY SMALLEST TOPOLOGICAL SORT
// Usage: When multiple valid topo orders exist, and we need the smallest lexicographically.
// Time: O((V + E) log V) | Space: O(V)
// ============================================================================
vector<int> lexicographicalTopoSort(int n) {
    // Min-Priority Queue to process the smallest numbered available node first
    priority_queue<int, vector<int>, greater<int>> pq;
    vector<int> topo;

    for (int i = 1; i <= n; i++) {
        if (in_degree[i] == 0) pq.push(i);
    }

    while (!pq.empty()) {
        int u = pq.top();
        pq.pop();
        topo.push_back(u);

        for (int v : adj[u]) {
            in_degree[v]--;
            if (in_degree[v] == 0) {
                pq.push(v);
            }
        }
    }

    if ((int)topo.size() < n) return {}; // Cycle detected
    return topo;
}
