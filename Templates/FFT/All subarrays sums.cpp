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

// Computes the frequency of all possible positive subarray sums
// Returns a vector where ans[S] is the number of subarrays that sum to S
vector<int> all_subarray_sums(const vector<int> &A) {
    if (A.empty()) return {};

    int n = A.size();
    vector<int> pref(n + 1, 0);
    for (int i = 0; i < n; ++i) {
        pref[i + 1] = pref[i] + A[i];
    }

    int max_pref = pref[n]; // Since elements are non-negative, pref[n] is maximum
    
    // poly1 represents P[j] for j from 0 to n
    vector<int> poly1(max_pref + 1, 0);
    // poly2 represents -P[i-1], which we flip by anchoring around max_pref
    vector<int> poly2(max_pref + 1, 0);

    for (int p : pref) {
        poly1[p]++;
        poly2[max_pref - p]++;
    }

    vector<int> res = multiply(poly1, poly2);

    // The subarray sum S = P[j] - P[i-1] lands at index: max_pref + S
    // We only care about positive subarray sums (S > 0)
    vector<int> ans(max_pref + 1, 0);
    for (int s = 1; s <= max_pref; ++s) {
        if (max_pref + s < (int)res.size()) {
            ans[s] = res[max_pref + s];
        }
    }

    return ans;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    // Subarrays: [1], [2], [3], [1,2]=3, [2,3]=5, [1,2,3]=6
    vector<int> A = {1, 2, 3}; 
    
    vector<int> sub_sums = all_subarray_sums(A);

    cout << "Subarray Sum Frequencies:\n";
    for (size_t i = 1; i < sub_sums.size(); ++i) {
        if (sub_sums[i] > 0) {
            cout << "Sum " << i << ": " << sub_sums[i] << " times\n";
        }
    }

    return 0;
}
