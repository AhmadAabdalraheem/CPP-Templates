#include <bits/stdc++.h>
using namespace std;

using ll = long long;
const ll oo = 2e18;

// ============================================================================
// 2D GRID DIJKSTRA
// Usage: Shortest path on a weighted 2D grid/matrix.
// Time: O((N * M) log(N * M)) | Space: O(N * M)
// ============================================================================
int dr[] = {-1, 1, 0, 0}; // Up, Down, Left, Right
int dc[] = {0, 0, -1, 1};

bool isValid(int r, int c, int n, int m) {
    return (r >= 0 && r < n && c >= 0 && c < m);
}

ll gridDijkstra(int sr, int sc, int er, int ec, const vector<vector<ll>>& grid) {
    int n = grid.size();
    int m = grid[0].size();

    vector<vector<ll>> dist(n, vector<ll>(m, oo));
    // min-heap storing {cost, {r, c}}
    priority_queue<pair<ll, pair<int, int>>, vector<pair<ll, pair<int, int>>>, greater<pair<ll, pair<int, int>>>> pq;

    dist[sr][sc] = grid[sr][sc];
    pq.push({grid[sr][sc], {sr, sc}});

    while (!pq.empty()) {
        auto [cost, pos] = pq.top();
        auto [r, c] = pos;
        pq.pop();

        if (cost > dist[r][c]) continue;
        if (r == er && c == ec) return dist[er][ec]; // Reached destination

        for (int i = 0; i < 4; i++) {
            int nr = r + dr[i];
            int nc = c + dc[i];

            if (isValid(nr, nc, n, m)) {
                ll w = grid[nr][nc]; // Weight of moving to neighbor cell
                if (dist[r][c] + w < dist[nr][nc]) {
                    dist[nr][nc] = dist[r][c] + w;
                    pq.push({dist[nr][nc], {nr, nc}});
                }
            }
        }
    }
    return dist[er][ec];
}

// Test Main
int main() {
    vector<vector<ll>> grid = {
        {1, 3, 1},
        {1, 5, 1},
        {4, 2, 1}
    };

    ll min_cost = gridDijkstra(0, 0, 2, 2, grid);
    cout << "Min Path Cost from (0,0) to (2,2): " << min_cost << " (Expected: 7)\n";
    return 0;
}
