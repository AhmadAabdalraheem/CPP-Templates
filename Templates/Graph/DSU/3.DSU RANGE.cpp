#include <bits/stdc++.h>
using namespace std;

// ============================================================================
// RANGE DSU (INTERVAL MERGING)
// Usage: Quickly merge all consecutive elements in range [L, R].
// Time: O(N alpha(N)) total across all range merges | Space: O(N)
// ============================================================================
struct RangeDSU {
    vector<int> parent; // parent[i] points to next UNMERGED element >= i

    void init(int n) {
        parent.resize(n + 2);
        iota(parent.begin(), parent.end(), 0);
    }

    int find_next(int i) {
        if (i == parent[i]) return i;
        return parent[i] = find_next(parent[i]); // Path compression
    }

    // Merge all elements in range [l, r]
    void merge_range(int l, int r) {
        l = find_next(l);
        while (l < r) {
            parent[l] = l + 1; // Mark 'l' as merged with 'l + 1'
            l = find_next(l);
        }
    }
};

// Test Main
int main() {
    RangeDSU dsu;
    dsu.init(10); // 10 elements

    dsu.merge_range(2, 5); // Merges elements 2, 3, 4, 5
    
    cout << "Next unmerged after 2: " << dsu.find_next(2) << " (Expected: 5)\n";
    cout << "Next unmerged after 4: " << dsu.find_next(4) << " (Expected: 5)\n";
    
    dsu.merge_range(4, 8); // Fast skips 4, 5 and merges 5, 6, 7, 8
    
    cout << "Next unmerged after 2: " << dsu.find_next(2) << " (Expected: 8)\n";
    cout << "Next unmerged after 7: " << dsu.find_next(7) << " (Expected: 8)\n";

    return 0;
}
