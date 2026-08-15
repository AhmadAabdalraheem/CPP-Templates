#include <bits/stdc++.h>
using namespace std;

/*
 * =============================================================================
 * TEMPLATE: Generic Dynamic Sparse Table
 * =============================================================================
 * USAGE:
 * - Precomputes static range queries over a fixed array without updates.
 * - Supports O(1) query for Idempotent operations (Min, Max, GCD, Bitwise AND/OR).
 * - Supports O(log N) query for Non-Idempotent operations (Range Sum, Range XOR).
 *
 * COMPLEXITY CHEAT SHEET:
 * -----------------------------------------------------------------------------
 * | Operation Type                | Example Functions    | Time Complexity    |
 * -----------------------------------------------------------------------------
 * | Build                         | -                    | O(N log N)         |
 * | Idempotent Query              | Min, Max, GCD, AND/OR| O(1)               |
 * | Non-Idempotent Query          | Range Sum, Range XOR | O(log N)           |
 * | Space Complexity              | Memory footprint     | O(N log N)         |
 * -----------------------------------------------------------------------------
 * =============================================================================
 */

struct SparseTable {
    int n, K;
    vector<vector<long long>> table;

    // Custom operation (Change according to problem: min, max, gcd, etc.)
    long long combine(long long v1, long long v2) {
        return max(v1, v2);
    }

    SparseTable() : n(0), K(0) {}

    // Builds the Sparse Table dynamically for any given vector
    // Time Complexity: O(N log N) | Space Complexity: O(N log N)
    void build(const vector<long long>& v) {
        n = v.size();
        if (n == 0) return;

        K = __lg(n) + 1; // Built-in O(1) Bitwise log2 logic
        table.assign(K, vector<long long>(n, 0));

        for (int i = 0; i < n; ++i) {
            table[0][i] = v[i];
        }

        for (int p = 1; p < K; ++p) {
            for (int i = 0; i + (1 << p) <= n; ++i) {
                table[p][i] = combine(table[p - 1][i], table[p - 1][i + (1 << (p - 1))]);
            }
        }
    }

    // -------------------------------------------------------------------------
    // 1. IDEMPOTENT QUERY -> Time Complexity: O(1)
    // Works for: Min, Max, GCD, Bitwise AND, Bitwise OR
    // -------------------------------------------------------------------------
    long long query1(int l, int r) {
        int p = __lg(r - l + 1); // O(1) Bitwise instruction
        return combine(table[p][l], table[p][r - (1 << p) + 1]);
    }

    // -------------------------------------------------------------------------
    // 2. NON-IDEMPOTENT QUERY -> Time Complexity: O(log N)
    // Works for: Range Sum, Range XOR, Range Product
    // -------------------------------------------------------------------------
    long long query2(int l, int r) {
        long long ans = 0; // Set appropriate neutral value if needed
        bool first = true;

        for (int p = K - 1; p >= 0; --p) {
            if ((1 << p) <= r - l + 1) {
                if (first) {
                    ans = table[p][l];
                    first = false;
                } else {
                    ans = combine(ans, table[p][l]); // Change for sum/xor
                }
                l += (1 << p);
            }
        }
        return ans;
    }
};

// ==================== USAGE EXAMPLE ====================
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    vector<long long> a = {1, 3, 8, 2, 9, 4, 7, 5};
    SparseTable st;
    
    // Build: O(N log N)
    st.build(a);

    cout << "=== SPARSE TABLE DEMO ===\n";

    // Query Idempotent Range Max: O(1)
    cout << "Range Max [2, 5]: " << st.query1(2, 5) << " | Complexity: O(1)\n"; // Expected: 9

    // Query Idempotent Range Max: O(1)
    cout << "Range Max [0, 3]: " << st.query1(0, 3) << " | Complexity: O(1)\n"; // Expected: 8

    // Query Non-Idempotent: O(log N)
    cout << "Non-Idempotent [1, 6]: " << st.query2(1, 6) << " | Complexity: O(log N)\n";

    return 0;
}
