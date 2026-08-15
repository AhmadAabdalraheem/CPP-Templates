#include <bits/stdc++.h>
using namespace std;

using ll = long long;
const int N = 2e5 + 5;

vector<int> g_rr[N];
ll subtree_sz[N];
ll sum_dist[N]; // sum_dist[u] = sum of distances from u to all nodes in its subtree
ll total_ans[N]; // total_ans[u] = sum of distances from u to ALL nodes in tree
int total_n;

void dfs1_reroot(int u, int p) {
    subtree_sz[u] = 1;
    sum_dist[u] = 0;

    for (int v : g_rr[u]) {
        if (v != p) {
            dfs1_reroot(v, u);
            subtree_sz[u] += subtree_sz[v];
            sum_dist[u] += sum_dist[v] + subtree_sz[v];
        }
    }
}

void dfs2_reroot(int u, int p) {
    for (int v : g_rr[u]) {
        if (v != p) {
            // Rerooting formula
            total_ans[v] = total_ans[u] - subtree_sz[v] + (total_n - subtree_sz[v]);
            dfs2_reroot(v, u);
        }
    }
}

void solveRerooting(int n) {
    total_n = n;
    dfs1_reroot(1, 0);

    total_ans[1] = sum_dist[1]; // Base answer for root = 1
    dfs2_reroot(1, 0);

    for (int i = 1; i <= n; i++) {
        cout << "Sum of distances from node " << i << ": " << total_ans[i] << "\n";
    }
}

// Test Main
int main() {
    int n = 3;
    // Tree: 1-2, 2-3
    g_rr[1] = {2}; g_rr[2] = {1, 3}; g_rr[3] = {2};

    solveRerooting(n);
    return 0;
}
