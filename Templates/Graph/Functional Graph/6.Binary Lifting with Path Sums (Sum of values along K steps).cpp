#include <bits/stdc++.h>
using namespace std;

using ll = long long;
const int N = 2e5 + 5;
const int LOG = 60;

int to_node[N];
ll val[N];
int up[N][LOG];
ll sum_val[N][LOG]; // sum_val[u][j] = sum of values on jumping 2^j steps from u

void buildBinaryLiftingWithSum(int n) {
    for (int i = 1; i <= n; i++) {
        up[i][0] = to_node[i];
        sum_val[i][0] = val[to_node[i]];
    }

    for (int j = 1; j < LOG; j++) {
        for (int i = 1; i <= n; i++) {
            up[i][j] = up[up[i][j - 1]][j - 1];
            sum_val[i][j] = sum_val[i][j - 1] + sum_val[up[i][j - 1]][j - 1];
        }
    }
}

// Returns {final_node, total_sum_including_start}
pair<int, ll> jumpAndSum(int node, ll k) {
    ll total_sum = val[node]; // Include starting node's value

    for (int j = 0; j < LOG; j++) {
        if ((k >> j) & 1) {
            total_sum += sum_val[node][j];
            node = up[node][j];
        }
    }
    return {node, total_sum};
}

// Test Main
int main() {
    int n = 3;
    to_node[1] = 2; val[1] = 10;
    to_node[2] = 3; val[2] = 20;
    to_node[3] = 1; val[3] = 30; // Cycle 1->2->3->1

    buildBinaryLiftingWithSum(n);

    auto [final_node, sum] = jumpAndSum(1, 2); // 1 -> 2 -> 3 (2 steps)
    cout << "Final Node: " << final_node << " (Expected: 3)\n";
    cout << "Total Sum (val[1]+val[2]+val[3]): " << sum << " (Expected: 60)\n";

    return 0;
}
