// 2D Grid BFS
int dr[] = {-1, 1, 0, 0};
int dc[] = {0, 0, -1, 1};

bool isValid(int r, int c, int n, int m) {
    return r >= 0 && r < n && c >= 0 && c < m;
}

void bfs2D(int sr, int sc, int n, int m, vector<vector<int>>& dist) {
    queue<pair<int, int>> q;
    q.push({sr, sc});
    dist[sr][sc] = 0;

    while (!q.empty()) {
        auto [r, c] = q.front();
        q.pop();

        for (int i = 0; i < 4; i++) {
            int nr = r + dr[i];
            int nc = c + dc[i];

            if (isValid(nr, nc, n, m) && dist[nr][nc] == -1) {
                dist[nr][nc] = dist[r][c] + 1;
                q.push({nr, nc});
            }
        }
    }
}
