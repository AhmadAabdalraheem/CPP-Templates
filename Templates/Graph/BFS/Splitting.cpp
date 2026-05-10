/*
    Split Into Two Sets

    Idea:
    - Represent numbers as nodes.
    - Each domino (u,v) is an edge.

    We need to divide edges into 2 sets such that:
    - In each set, every number appears at most once.

    Observations:
    1) Any node cannot appear more than 2 times.
       Because there are only 2 sets.
       => degree[node] <= 2

    2) Self loop (u == v) is impossible.
       Because the same number appears twice in one domino.

    3) Graph must be bipartite.
       Why?
       - Paths work.
       - Even cycles work.
       - Odd cycles cannot alternate between 2 sets.

    So:
    - self loop => NO
    - degree > 2 => NO
    - odd cycle => NO

    Use this idea when:
    - You need to split edges into 2 groups.
    - Adjacent edges must belong to different groups.
    - Graph checking / bipartite problems.
*/

#include <bits/stdc++.h>
using namespace std;

void solve() {

    int n;
    cin >> n;

    vector<vector<int>> adj(n + 1);
    vector<int> deg(n + 1);
    vector<int> color(n + 1, -1);

    for (int i = 0; i < n; i++) {

        int u, v;
        cin >> u >> v;

        if (u == v) {
            no;
        }

        adj[u].push_back(v);
        adj[v].push_back(u);

        deg[u]++;
        deg[v]++;

        if (deg[u] > 2 || deg[v] > 2) {
            no;
        }
    }

    for (int i = 1; i <= n; i++) {

        if (color[i] != -1) {
            continue;
        }

        queue<int> q;

        q.push(i);
        color[i] = 0;

        while (!q.empty()) {

            int node = q.front();
            q.pop();

            for (auto v : adj[node]) {

                if (color[v] == -1) {

                    color[v] = color[node] ^ 1;
                    q.push(v);
                }

                else if (color[v] == color[node]) {
                    no;
                }
            }
        }
    }

    yes;
}

signed main() {

    Fast

    int t = 1;
    cin >> t;

    while (t--) {
        solve();
    }
}
