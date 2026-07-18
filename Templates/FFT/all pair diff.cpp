#include <bits/stdc++.h>
using namespace std;

using ll = long long;

int mod = 998244353; 
int root = 3; 

int modpow(int b, int e, int m) {
    int ans = 1;
    for (; e; b = (ll)b * b % m, e /= 2)
        if (e & 1) ans = (ll)ans * b % m;
    return ans;
}

void ntt(vector<int> &a) {
    int n = (int)a.size();
    int L = 31 - __builtin_clz(n);
    
    static vector<int> rt(2, 1); 
    if ((int)rt.size() < n) {
        int k = (int)rt.size(), s = 31 - __builtin_clz(k * 2);
        rt.resize(n);
        for (; k < n; k *= 2, s++) { 
            int z[] = {1, modpow(root, mod >> s, mod)};
            for (int i = k; i < 2 * k; ++i) {
                rt[i] = (ll)rt[i / 2] * z[i & 1] % mod;
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
                int z = (ll)rt[j + k] * a[i + j + k] % mod;
                int &ai = a[i + j];
                a[i + j + k] = ai - z + (z > ai ? mod : 0);
                ai += (ai + z >= mod ? z - mod : z);
            }
        }
    }
}

vector<int> multiply(const vector<int> &a, const vector<int> &b) {
    if (a.empty() || b.empty()) return {};
    
    int s = (int)a.size() + (int)b.size() - 1;
    int B = 32 - __builtin_clz(s);
    if ((1 << (B - 1)) == s) B--; 
    int n = 1 << B;
    int inv = modpow(n, mod - 2, mod);
    
    vector<int> L(n, 0), R(n, 0), out(n);
    for (size_t i = 0; i < a.size(); ++i) L[i] = a[i];
    for (size_t i = 0; i < b.size(); ++i) R[i] = b[i];
    
    ntt(L);
    ntt(R);
    
    for (int i = 0; i < n; i++) {
        out[-i & (n - 1)] = (ll)L[i] * R[i] % mod * inv % mod;
    }
    ntt(out);
    
    return {out.begin(), out.begin() + s};
}

// 1. All Pairs Differences between TWO distinct arrays: A[i] - B[j]
// The difference D = A[i] - B[j] can be negative. 
// The frequency count of difference D is stored at index: max_B + D
vector<int> all_pairs_diff_two_arrays(const vector<int> &A, const vector<int> &B, int &offset) {
    if (A.empty() || B.empty()) return {};
    
    int max_A = *max_element(A.begin(), A.end());
    int max_B = *max_element(B.begin(), B.end());
    
    vector<int> polyA(max_A + 1, 0);
    vector<int> polyB(max_B + 1, 0);
    
    for (int x : A) polyA[x]++;
    // Flip polynomial B to change addition to subtraction
    for (int x : B) polyB[max_B - x]++;
    
    offset = max_B; // Difference D matches result index (offset + D)
    return multiply(polyA, polyB);
}

// 2. All Pairs Differences within the SAME array: A[i] - A[j] for i != j
// The difference D = A[i] - A[j] can be negative.
// The frequency count of difference D is stored at index: max_A + D
vector<int> all_pairs_diff_same_array(const vector<int> &A, int &offset) {
    if (A.size() < 2) return {};
    
    int max_A = *max_element(A.begin(), A.end());
    vector<int> poly(max_A + 1, 0);
    for (int x : A) poly[x]++;
    
    vector<int> poly_rev(max_A + 1, 0);
    for (int x : A) poly_rev[max_A - x]++;
    
    vector<int> res = multiply(poly, poly_rev);
    
    offset = max_A; // Difference D matches result index (offset + D)
    
    // Remove self-matching differences: A[i] - A[i] = 0 (which lands at index offset)
    res[offset] = (res[offset] - (int)A.size() + mod) % mod;
    
    return res;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    // Scenario 1: Two Distinct Arrays
    vector<int> A = {5, 2};
    vector<int> B = {1, 3};
    int offset_two;
    vector<int> res_two = all_pairs_diff_two_arrays(A, B, offset_two);
    
    cout << "Differences (A[i] - B[j]):\n";
    for (int idx = 0; idx < (int)res_two.size(); ++idx) {
        if (res_two[idx] > 0) {
            int diff = idx - offset_two;
            cout << "Difference " << diff << ": " << res_two[idx] << " times\n";
        }
    }

    cout << "\n-----------------------------\n\n";

    // Scenario 2: Within Same Array (i != j)
    vector<int> C = {1, 4, 5}; 
    // Differences should be: 1-4=-3, 1-5=-4, 4-1=3, 4-5=-1, 5-1=4, 5-4=1
    int offset_same;
    vector<int> res_same = all_pairs_diff_same_array(C, offset_same);
    
    cout << "Differences within same array (C[i] - C[j] for i != j):\n";
    for (int idx = 0; idx < (int)res_same.size(); ++idx) {
        if (res_same[idx] > 0) {
            int diff = idx - offset_same;
            cout << "Difference " << diff << ": " << res_same[idx] << " times\n";
        }
    }

    return 0;
}
