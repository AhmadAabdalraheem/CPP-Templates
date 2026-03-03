#include <bits/stdc++.h>
using namespace std;

#define fast ios::sync_with_stdio(false); cin.tie(nullptr);

/*
=================  Strongly Connected Components  =================

Kosaraju's Algorithm (O(n + m))

Main Idea:
1) Run DFS on original graph.
   - Push node to "order" AFTER finishing all children.
   - This sorts nodes by finishing time (topological order of SCC graph).

2) Reverse the graph.

3) Process nodes in reverse finishing order.
   - Run DFS on reversed graph.
   - Each DFS tree gives one SCC.

*/

const int N = 200005;

vector<int> adj[N], radj[N];  // original graph + reversed graph
vector<int> order;           // stores nodes by finishing time
bool visited[N];
int comp[N];                 // component id of each node

int n, m;

void dfs1(int u) {
    visited[u] = true;
    for (int v : adj[u])
        if (!visited[v])
            dfs1(v);

    // push AFTER visiting children -> finishing time order
    order.push_back(u);
}

void dfs2(int u, int c) {
    comp[u] = c;
    for (int v : radj[u])
        if (comp[v] == -1)
            dfs2(v, c);
}

int main() {
    fast

    cin >> n >> m;

    for (int i = 1; i <= n; i++) {
        adj[i].clear();
        radj[i].clear();
        visited[i] = false;
        comp[i] = -1;
    }

    for (int i = 0; i < m; i++) {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);   // original edge
        radj[v].push_back(u);  // reversed edge
    }

    // Step 1: get nodes ordered by finishing time
    for (int i = 1; i <= n; i++)
        if (!visited[i])
            dfs1(i);

    // Step 2: process in reverse finishing order
    reverse(order.begin(), order.end());

    int scc = 0;

    // Step 3: run DFS on reversed graph
    for (int u : order)
        if (comp[u] == -1)
            dfs2(u, scc++);

    cout << scc << "\n";   // number of strongly connected components

    return 0;
}
