#include <bits/stdc++.h>
using namespace std;

// ============================================================================
// TREE DIAMETER & PATH RECONSTRUCTION (2-BFS METHOD)
// Usage: Finds tree diameter length, its endpoints (A, B), and the exact path.
// Time: O(N) | Space: O(N)
// ============================================================================
const int N = 2e5 + 5;

vector<int> tree_adj[N];

pair<int, vector<int>> getFarthestAndDist(int start, int n) {
    vector<int> dist(n + 1, -1);
    queue<int> q;

    q.push(start);
    dist[start] = 0;
    int far_node = start;

    while (!q.empty()) {
        int u = q.front(); q.pop();

        if (dist[u] > dist[far_node]) {
            far_node = u;
        }

        for (int v : tree_adj[u]) {
            if (dist[v] == -1) {
                dist[v] = dist[u] + 1;
                q.push(v);
            }
        }
    }
    return {far_node, dist};
}

void getDiameterInfo(int n) {
    // 1. Run BFS from node 1 to find endpoint A
    int nodeA = getFarthestAndDist(1, n).first;

    // 2. Run BFS from A to find endpoint B and distances from A
    auto [nodeB, distA] = getFarthestAndDist(nodeA, n);

    int diameter_length = distA[nodeB];

    // 3. Reconstruct path from A to B
    vector<int> path;
    int cur = nodeB;
    path.push_back(cur);

    while (cur != nodeA) {
        for (int v : tree_adj[cur]) {
            if (distA[v] == distA[cur] - 1) {
                cur = v;
                path.push_back(cur);
                break;
            }
        }
    }
    reverse(path.begin(), path.end());

    cout << "Diameter Length: " << diameter_length << " (Between " << nodeA << " and " << nodeB << ")\n";
    cout << "Diameter Path: ";
    for (int u : path) cout << u << " ";
    cout << "\n";
}

// Test Main
int main() {
    int n = 5;
    // Tree: 1-2, 2-3, 3-4, 4-5
    tree_adj[1] = {2}; tree_adj[2] = {1, 3};
    tree_adj[3] = {2, 4}; tree_adj[4] = {3, 5}; tree_adj[5] = {4};

    getDiameterInfo(n);
    return 0;
}
