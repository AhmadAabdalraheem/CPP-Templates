#include <bits/stdc++.h>
#define Fast ios_base::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr);
using namespace std;

vector<vector<int>> g;
vector<int> dist;

int bfs(int start) {
    queue<int> q;
    q.push(start);
    dist[start] = 0;

    int far = start;

    while (!q.empty()) {
        int cur = q.front();
        q.pop();

        for (auto it : g[cur]) {
            if (dist[it] == -1) {
                dist[it] = dist[cur] + 1;
                q.push(it);

                if (dist[it] > dist[far])
                    far = it;
            }
        }
    }
    return far;
}

int main() {
    Fast

    int n;
    cin >> n;

    g.resize(n + 1);

    for (int i = 1; i < n; i++) {
        int u, v;
        cin >> u >> v;
        g[u].push_back(v);
        g[v].push_back(u);
    }

    dist.assign(n + 1, -1);
    int a = bfs(1);        // أبعد نود من 1

    dist.assign(n + 1, -1);
    int b = bfs(a);        // أبعد نود من a

    cout << dist[b] << endl; // طول القطر
}
