#include <bits/stdc++.h>
using namespace std;

#define ll long long
const int N = 2e5 + 5;

vector<int> adj[N];
int subtree[N], depth[N];

void dfs(int node, int parent) {
    subtree[node] = 1;
    for (int child : adj[node]) {
        if (child == parent) continue;
        depth[child] = depth[node] + 1;
        dfs(child, node);
        subtree[node] += subtree[child];
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    for (int i = 1; i < n; i++) {
        int p;
        cin >> p;
        p--;
        adj[p].push_back(i);
        adj[i].push_back(p); // خليها generic لأي tree
    }

    dfs(0, -1);

    for (int i = 0; i < n; i++) {
        cout << subtree[i] - 1 << " ";
    }

    return 0;
}
