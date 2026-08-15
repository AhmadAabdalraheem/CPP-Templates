#include <bits/stdc++.h>
using namespace std;

// ============================================================================
// FULL FUNCTIONAL GRAPH DECOMPOSITION O(N)
// Usage: Classifies all nodes into Cycles or Trees leading to Cycles.
// Time: O(N) | Space: O(N)
// ============================================================================
const int N = 2e5 + 5;

int to_node[N];
int vis[N];            // 0: Unvisited, 1: Visiting (in current path), 2: Processed
bool in_cycle[N];      // Is the node part of a cycle?
int cycle_id[N];       // ID of the cycle this node belongs/leads to
int cycle_idx[N];      // Position/Index of node inside its cycle (0 to len-1)
int dist_to_cycle[N];  // Steps needed to reach the cycle (0 if in cycle)
vector<int> cycle_len; // Length of cycle 'id'

int timer = 0;

void decompose(int n) {
    fill(vis + 1, vis + n + 1, 0);
    fill(in_cycle + 1, in_cycle + n + 1, false);
    fill(dist_to_cycle + 1, dist_to_cycle + n + 1, 0);
    cycle_len.clear();

    int num_cycles = 0;

    for (int i = 1; i <= n; i++) {
        if (vis[i] != 0) continue;

        // Step 1: Trace path to find new cycle or hit processed component
        vector<int> path;
        int cur = i;
        while (cur != 0 && vis[cur] == 0) {
            vis[cur] = 1; // Mark as visiting in current path
            path.push_back(cur);
            cur = to_node[cur];
        }

        // Case A: Found a NEW cycle in the current path
        if (cur != 0 && vis[cur] == 1) {
            int c_id = num_cycles++;
            vector<int> cyc;
            
            // Extract cycle nodes back to 'cur'
            while (!path.empty() && path.back() != cur) {
                cyc.push_back(path.back());
                path.pop_back();
            }
            cyc.push_back(cur);
            path.pop_back();
            reverse(cyc.begin(), cyc.end());

            int len = cyc.size();
            cycle_len.push_back(len);

            for (int idx = 0; idx < len; idx++) {
                int u = cyc[idx];
                in_cycle[u] = true;
                cycle_id[u] = c_id;
                cycle_idx[u] = idx;
                dist_to_cycle[u] = 0;
                vis[u] = 2;
            }
        }

        // Case B: Process nodes outside the cycle (Trees pointing to cycle)
        while (!path.empty()) {
            int u = path.back();
            path.pop_back();

            int nxt = to_node[u];
            cycle_id[u] = cycle_id[nxt];
            dist_to_cycle[u] = dist_to_cycle[nxt] + 1;
            vis[u] = 2;
        }
    }
}

// Test Main
int main() {
    int n = 6;
    // Graph: 1->2, 2->3, 3->2 (Cycle 2-3), 4->1, 5->6, 6->5 (Cycle 5-6)
    to_node[1] = 2;
    to_node[2] = 3;
    to_node[3] = 2;
    to_node[4] = 1;
    to_node[5] = 6;
    to_node[6] = 5;

    decompose(n);

    cout << "Is Node 2 in cycle? " << in_cycle[2] << " (Expected: 1)\n";
    cout << "Is Node 4 in cycle? " << in_cycle[4] << " (Expected: 0)\n";
    cout << "Dist from Node 4 to cycle: " << dist_to_cycle[4] << " (Expected: 2)\n";
    cout << "Cycle length for Node 4: " << cycle_len[cycle_id[4]] << " (Expected: 2)\n";

    return 0;
}
