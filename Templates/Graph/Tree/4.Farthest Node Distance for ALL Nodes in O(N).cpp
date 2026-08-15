#include <bits/stdc++.h>
using namespace std;

// ============================================================================
// ALL-NODES FARTHEST DISTANCE O(N)
// Usage: Calculates maximum distance to ANY node for ALL vertices in O(N).
// Time: O(N) | Space: O(N)
// ============================================================================
const int N = 2e5 + 5;
vector<int> g_far[N];

vector<int> getDistancesFrom(int start, int n) {
    vector<int> dist(n + 1, -1);
    queue<int> q;
    q.push(start);
    dist[start] = 0;

    while (!q.empty()) {
        int u = q.front(); q.pop();
        for (int v : g_far[u]) {
            if (dist[v] == -1) {
                dist[v] = dist[u] + 1;
                q.push(v);
            }
        }
    }
    return dist;
}

void computeAllFarthestDistances(int n) {
    // 1. Find Diameter Endpoints A and B
    int nodeA = 1;
    auto dist1 = getDistancesFrom(1, n);
    for (int i = 1; i <= n; i++) if (dist1[i] > dist1[nodeA]) nodeA = i;

    int nodeB = nodeA;
    auto distA = getDistancesFrom(nodeA, n);
    for (int i = 1; i <= n; i++) if (distA[i] > distA[nodeB]) nodeB = i;

    auto distB = getDistancesFrom(nodeB, n);

    // 2. Max distance for each node is max(distA[i], distB[i])
    cout << "Farthest distances for all nodes:\n";
    for (int i = 1; i <= n; i++) {
        int max_d = max(distA[i], distB[i]);
        cout << "Node " << i << ": " << max_d << "\n";
    }
}

// Test Main
int main() {
    int n = 5;
    // Tree: 1-2-3-4-5
    g_far[1] = {2}; g_far[2] = {1, 3}; g_far[3] = {2, 4}; g_far[4] = {3, 5}; g_far[5] = {4};

    computeAllFarthestDistances(n);
    return 0;
}
