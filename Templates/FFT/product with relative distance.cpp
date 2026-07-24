#include <bits/stdc++.h>
using namespace std;

/*
 * ============================================================================
 * 📦 TEMPLATE OVERVIEW: Shifted Cross-Correlation via NTT
 * ============================================================================
 * 🎯 PROBLEM:
 *    Given two 0-indexed arrays A (size N) and B (size M), process queries for 
 *    a relative distance shift 'x':
 *      Query(x) = Sum_{i} ( A[i] * B[i + x] )
 *    where 0 <= i < N and 0 <= i + x < M.
 *
 * 🎯 ALGORITHM:
 *    1. Reverse array A -> A_rev[i] = A[N - 1 - i].
 *    2. Multiply A_rev and B using NTT -> Conv.
 *    3. For any shift 'x', the result is at index: k = x + N - 1.
 *
 * ⏱️ COMPLEXITY:
 *    • Preprocessing: O((N + M) * log(N + M))
 *    • Per Query: O(1)
 * ============================================================================
 */

using ll = long long;

const int MOD = 998244353;
const int ROOT = 3;

// Fast Modular Exponentiation
int modpow(int b, int e, int m = MOD) {
    int ans = 1;
    for (; e; b = (ll)b * b % m, e /= 2)
        if (e & 1) ans = (ll)ans * b % m;
    return ans;
}

// In-place Number Theoretic Transform (NTT)
void ntt(vector<int> &a) {
    int n = (int)a.size();
    int L = 31 - __builtin_clz(n);
    
    static vector<int> rt(2, 1); 
    if ((int)rt.size() < n) {
        int k = (int)rt.size(), s = 31 - __builtin_clz(k * 2);
        rt.resize(n);
        for (; k < n; k *= 2, s++) { 
            int z[] = {1, modpow(ROOT, MOD >> s, MOD)};
            for (int i = k; i < 2 * k; ++i) {
                rt[i] = (ll)rt[i / 2] * z[i & 1] % MOD;
            }
        }
    }
    
    vector<int> rev(n);
    for (int i = 0; i < n; ++i) {
        rev[i] = (rev[i / 2] | (i & 1) << L) / 2;
    }
    
    for (int i = 0; i < n; ++i) {
        if (i < rev[i]) swap(a[i], a[rev[i]]);
    }
    
    for (int k = 1; k < n; k *= 2) {
        for (int i = 0; i < n; i += 2 * k) {
            for (int j = 0; j < k; ++j) {
                int z = (ll)rt[j + k] * a[i + j + k] % MOD;
                int &ai = a[i + j];
                a[i + j + k] = ai - z + (z > ai ? MOD : 0);
                ai += (ai + z >= MOD ? z - MOD : z);
            }
        }
    }
}

// Polynomial Multiplication via NTT
vector<int> multiply(const vector<int> &a, const vector<int> &b) {
    if (a.empty() || b.empty()) return {};
    
    int s = (int)a.size() + (int)b.size() - 1;
    int B = 32 - __builtin_clz(s);
    if ((1 << (B - 1)) == s) B--; 
    int n = 1 << B;
    int inv = modpow(n, MOD - 2, MOD);
    
    vector<int> L(n, 0), R(n, 0), out(n);
    for (size_t i = 0; i < a.size(); ++i) L[i] = (a[i] % MOD + MOD) % MOD;
    for (size_t i = 0; i < b.size(); ++i) R[i] = (b[i] % MOD + MOD) % MOD;
    
    ntt(L);
    ntt(R);
    
    for (int i = 0; i < n; i++) {
        out[-i & (n - 1)] = (ll)L[i] * R[i] % MOD * inv % MOD;
    }
    ntt(out);
    
    return {out.begin(), out.begin() + s};
}

// Structure to build precomputed correlation results
struct ShiftedDotProduct {
    int N, M;
    vector<int> conv;

    // Precompute O((N+M) log(N+M))
    ShiftedDotProduct(const vector<int> &A, const vector<int> &B) {
        N = A.size();
        M = B.size();

        // Reverse A to turn correlation into convolution
        vector<int> A_rev(A.rbegin(), A.rend());

        // conv[k] = Sum_{i} A_rev[i] * B[k - i]
        conv = multiply(A_rev, B);
    }

    // Query O(1): Computes Sum_{i} ( A[i] * B[i + x] )
    int query(int x) const {
        // Target index in convolution array: k = x + N - 1
        int k = x + N - 1;

        // Check if shift x yields overlapping valid indices
        if (k < 0 || k >= (int)conv.size()) {
            return 0; // No overlapping elements
        }
        return conv[k];
    }
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    // Example Arrays
    vector<int> A = {1, 2, 3}; // Size N = 3
    vector<int> B = {4, 5, 6, 7}; // Size M = 4

    // Initialize Precomputation
    ShiftedDotProduct solver(A, B);

    // Queries for shift x:
    // x = 0: A[0]*B[0] + A[1]*B[1] + A[2]*B[2] = 1*4 + 2*5 + 3*6 = 4 + 10 + 18 = 32
    // x = 1: A[0]*B[1] + A[1]*B[2] + A[2]*B[3] = 1*5 + 2*6 + 3*7 = 5 + 12 + 21 = 38
    // x = -1: A[1]*B[0] + A[2]*B[1]            = 2*4 + 3*5        = 8 + 15      = 23

    vector<int> queries = {0, 1, -1, 2, -2, 10};

    cout << "Query Results (Mod " << MOD << "):\n";
    for (int x : queries) {
        cout << "Shift x = " << setw(2) << x << " -> Sum: " << solver.query(x) << "\n";
    }

    return 0;
}
