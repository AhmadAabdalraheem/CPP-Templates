#include <bits/stdc++.h>
using namespace std;

using ll = long long;
const int N = 2e5 + 5;
const int LOG = 60; // Supports up to K <= 2^60 (~10^18)

int to_node[N];
int up[N][LOG];

void buildBinaryLifting(int n) {
    for (int i = 1; i <= n; i++) {
        up[i][0] = to_node[i];
    }

    for (int j = 1; j < LOG; j++) {
        for (int i = 1; i <= n; i++) {
            up[i][j] = up[up[i][j - 1]][j - 1];
        }
    }
}

// Move K steps from 'node' in O(log K)
int jump(int node, ll k) {
    for (int j = 0; j < LOG; j++) {
        if ((k >> j) & 1) {
            node = up[node][j];
        }
    }
    return node;
}

// Test Main
int main() {
    int n = 4;
    to_node[1] = 2;
    to_node[2] = 3;
    to_node[3] = 4;
    to_node[4] = 2; // Cycle 2->3->4->2

    buildBinaryLifting(n);

    cout << "Node 1 after 2 steps: " << jump(1, 2) << " (Expected: 3)\n";
    cout << "Node 1 after 10 steps: " << jump(1, 10) << " (Expected: 4)\n";

    return 0;
}
