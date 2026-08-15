// BFS template (Shortest Path on Unweighted Graph)
// O(V + E) time, O(V) space

const int N = 2e5 + 5;
vector<int> adj[N];
int dist[N];

void bfs(int start) {
    memset(dist, -1, sizeof(dist));
    queue<int> q;
    
    q.push(start);
    dist[start] = 0;
    
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
