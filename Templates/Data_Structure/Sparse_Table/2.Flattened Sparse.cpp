#include <bits/stdc++.h>
using namespace std;

/*
 * =============================================================================
 * TEMPLATE: Flattened 1D Fast Sparse Table
 * =============================================================================
 *
 *  WHEN TO USE FLATTENED 1D OVER STANDARD 2D SPARSE TABLE?
 * -----------------------------------------------------------------------------
 * 1. Tight Time Limits: When Time Limit is extremely tight (TL <= 0.3s - 0.5s)
 *    and number of queries is massive (Q >= 10^6).
 * 2. High Cache Performance: Storing data in a continuous 1D block guarantees
 *    CPU Cache Locality, reducing Cache Misses by up to 20-30%.
 * 3. Memory Optimization: Eliminates vector-of-vectors pointer overhead.
 *
 * ⚠️ WHEN TO STICK TO STANDARD 2D SPARSE TABLE?
 * -----------------------------------------------------------------------------
 * - For 99% of regular problems! Standard 2D table[p][i] is safer, simpler to
 *   write, and prevents 1D indexing math bugs (p * n + i) during contests.
 * =============================================================================
 */

struct FlattenedSparseTable {
    int n, K;
    vector<long long> table; // Single continuous memory block

    // Modify combine operation based on problem (min, max, std::gcd, etc.)
    long long combine(long long v1, long long v2) {
        return max(v1, v2);
    }

    FlattenedSparseTable() : n(0), K(0) {}

    // Build Time: O(N log N) | Space: O(N log N)
    void build(const vector<long long>& v) {
        n = v.size();
        if (n == 0) return;

        K = __lg(n) + 1; // O(1) Bitwise log2
        table.assign(K * n, 0);

        for (int i = 0; i < n; ++i) {
            table[i] = v[i]; // Row 0
        }

        for (int p = 1; p < K; ++p) {
            int prev_offset = (p - 1) * n;
            int curr_offset = p * n;
            int shift = 1 << (p - 1);

            for (int i = 0; i + (1 << p) <= n; ++i) {
                table[curr_offset + i] = combine(
                    table[prev_offset + i], 
                    table[prev_offset + i + shift]
                );
            }
        }
    }

    // -------------------------------------------------------------------------
    // 1. IDEMPOTENT QUERY -> Complexity: O(1)
    // Works for: Min, Max, GCD, Bitwise AND/OR
    // -------------------------------------------------------------------------
    long long query_idempotent(int l, int r) {
        int p = __lg(r - l + 1); // O(1) Bitwise instruction
        int offset = p * n;
        return combine(table[offset + l], table[offset + r - (1 << p) + 1]);
    }

    // -------------------------------------------------------------------------
    // 2. NON-IDEMPOTENT QUERY -> Complexity: O(log N)
    // Works for: Range Sum, Range XOR, Range Product
    // -------------------------------------------------------------------------
    long long query_non_idempotent(int l, int r) {
        long long ans = 0;
        bool first = true;

        for (int p = K - 1; p >= 0; --p) {
            if ((1 << p) <= r - l + 1) {
                long long val = table[p * n + l];
                if (first) {
                    ans = val;
                    first = false;
                } else {
                    ans = combine(ans, val); // Change to + or ^ if needed
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
    FlattenedSparseTable st;
    
    st.build(a); // O(N log N)

    cout << "=== FLATTENED SPARSE TABLE DEMO ===\n";
    cout << "Range Max [2, 5]: " << st.query_idempotent(2, 5) << " (Expected: 9)\n";
    cout << "Range Max [0, 3]: " << st.query_idempotent(0, 3) << " (Expected: 8)\n";

    return 0;
}
