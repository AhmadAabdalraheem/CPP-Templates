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

// 1. All Pairs Sums between TWO distinct arrays: A[i] + B[j]
// Returns a vector where ans[S] is the number of pairs that sum to S
vector<int> all_pairs_sums_two_arrays(const vector<int> &A, const vector<int> &B) {
    if (A.empty() || B.empty()) return {};
    
    int max_A = *max_element(A.begin(), A.end());
    int max_B = *max_element(B.begin(), B.end());
    
    vector<int> polyA(max_A + 1, 0);
    vector<int> polyB(max_B + 1, 0);
    
    for (int x : A) polyA[x]++;
    for (int x : B) polyB[x]++;
    
    return multiply(polyA, polyB);
}

// 2. All Pairs Sums within the SAME array: A[i] + A[j] for i < j
// Returns a vector where ans[S] is the number of unique pairs (i, j) with i < j that sum to S
vector<long long> all_pairs_sums_same_array(const vector<int> &A) {
    if (A.size() < 2) return {};
    
    int max_A = *max_element(A.begin(), A.end());
    vector<int> poly(max_A + 1, 0);
    for (int x : A) poly[x]++;
    
    // Result includes identity pairings A[i]+A[i] and duplicates (A[i]+A[j] and A[j]+A[i])
    vector<int> raw_conv = multiply(poly, poly);
    
    vector<long long> ans(raw_conv.size(), 0);
    for (size_t i = 0; i < raw_conv.size(); ++i) {
        ans[i] = raw_conv[i];
    }
    
    // Remove self-pairing elements (A[i] + A[i])
    for (int x : A) {
        if (x * 2 < (int)ans.size()) {
            ans[x * 2]--;
        }
    }
    
    // Since (A[i]+A[j]) and (A[j]+A[i]) were both counted, divide by 2 for unique combinations
    int inv2 = modpow(2, mod - 2, mod);
    for (size_t i = 0; i < ans.size(); ++i) {
        ans[i] = (ans[i] % mod + mod) % mod;
        ans[i] = (ans[i] * inv2) % mod;
    }
    
    return ans;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    // Scenario 1: Two Distinct Arrays
    vector<int> A = {1, 3, 5};
    vector<int> B = {2, 3};
    vector<int> res_two = all_pairs_sums_two_arrays(A, B);
    
    cout << "Pairs Sums (A[i] + B[j]):\n";
    for (size_t i = 0; i < res_two.size(); ++i) {
        if (res_two[i] > 0) {
            cout << "Sum " << i << ": " << res_two[i] << " times\n";
        }
    }

    cout << "\n-----------------------------\n\n";

    // Scenario 2: Same Array (i < j)
    vector<int> C = {1, 2, 2, 4}; // pairs: (1+2)=3, (1+2)=3, (1+4)=5, (2+2)=4, (2+4)=6, (2+4)=6
    vector<long long> res_same = all_pairs_sums_same_array(C);
    
    cout << "Pairs Sums within same array (C[i] + C[j] for i < j):\n";
    for (size_t i = 0; i < res_same.size(); ++i) {
        if (res_same[i] > 0) {
            cout << "Sum " << i << ": " << res_same[i] << " times\n";
        }
    }

    return 0;
}
