// ============================================================================
// TOPOLOGICAL SORT USING IN-DEGREES (KAHN'S ALGORITHM)
// Usage: Ordering tasks with dependencies & Cycle Detection in Directed Graphs.
// Time: O(V + E) | Space: O(V)
// ============================================================================
const int N = 2e5 + 5;
vector<int> adj[N];
int in_degree[N];

vector<int> topoSort(int n) {
    queue<int> q;
    vector<int> topo;

    // 1. Push all nodes with 0 in-degree (no prerequisites)
    for (int i = 1; i <= n; i++) {
        if (in_degree[i] == 0) {
            q.push(i);
        }
    }

    // 2. Process BFS
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        topo.push_back(u);

        for (int v : adj[u]) {
            in_degree[v]--; // Remove edge (u -> v)
            if (in_degree[v] == 0) {
                q.push(v); // 'v' is now free of dependencies
            }
        }
    }

    // 3. Cycle Detection Trick:
    // If topo.size() < n, there is a CYCLE! (Some nodes never reached in_degree = 0)
    if ((int)topo.size() < n) return {}; 

    return topo;
}
