#include <bits/stdc++.h>
using namespace std;

// ============================================================================
// CAN REACH & DISTANCE QUERY BETWEEN U AND V
// Usage: Determines if path u -> v exists and calculates exact steps.
// Time: O(log N) | Space: O(N log N)
// ============================================================================

// Assumes 'decompose()' and 'buildBinaryLifting()' are run
ll getDistance(int u, int v, int n) {
    // Case 1: Both nodes must belong to the same component/cycle
    if (cycle_id[u] != cycle_id[v]) return -1; // Unreachable

    // Case 2: Destination 'v' is IN THE CYCLE
    if (in_cycle[v]) {
        ll steps_to_cycle = dist_to_cycle[u];
        int entry_node = jump(u, steps_to_cycle); // Reach the cycle
        
        int len = cycle_len[cycle_id[u]];
        ll cycle_steps = (cycle_idx[v] - cycle_idx[entry_node] + len) % len;
        
        return steps_to_cycle + cycle_steps;
    }

    // Case 3: Destination 'v' is OUTSIDE the cycle (On the tree)
    // 'v' must be an ancestor of 'u' towards the cycle
    if (dist_to_cycle[u] < dist_to_cycle[v]) return -1; // 'v' is deeper, unreachable

    ll diff = dist_to_cycle[u] - dist_to_cycle[v];
    if (jump(u, diff) == v) {
        return diff;
    }

    return -1; // Unreachable
}
