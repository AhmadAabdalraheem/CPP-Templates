/*
 * =============================================================================
 * TRICK 1: Sparse Table + Binary Search for Distinct Range GCDs
 * =============================================================================
 * USAGE:
 * - Iterates over all distinct range GCDs in O(N * log(N) * log(MAX_A)).
 * =============================================================================
 */
void count_distinct_gcds(const vector<long long>& a, SparseTable& st) {
    int n = a.size();
    map<long long, long long> gcd_freq;

    for (int l = 0; l < n; ++l) {
        int curr_r = l;
        while (curr_r < n) {
            long long current_gcd = st.query_idempotent(l, curr_r);

            // Binary search to find the furthest index 'low' with the exact same GCD
            int low = curr_r, high = n - 1, best_r = curr_r;
            while (low <= high) {
                int mid = low + (high - low) / 2;
                if (st.query_idempotent(l, mid) == current_gcd) {
                    best_r = mid;
                    low = mid + 1; // Try to extend right
                } else {
                    high = mid - 1;
                }
            }

            // Number of subarrays starting at 'l' with value 'current_gcd' is (best_r - curr_r + 1)
            gcd_freq[current_gcd] += (best_r - curr_r + 1);
            curr_r = best_r + 1; // Jump directly to the next distinct GCD segment
        }
    }
}
