#include <bits/stdc++.h>
using namespace std;

using ll = long long;

// =============================================================================
// TEMPLATE: Full Exact NTT & Polynomial Exponentiation (Unclamped Degree)
// =============================================================================
// TIME COMPLEXITY  : O( (N * P) log (N * P) )
// SPACE COMPLEXITY : O(N * P)
// MODULO           : 998244353
// =============================================================================

const int MOD = 998244353;
const int ROOT = 3;

int modpow(int b, int e, int m = MOD) {
    int ans = 1;
    for (; e; b = (ll)b * b % m, e /= 2)
        if (e & 1) ans = (ll)ans * b % m;
    return ans;
}

// In-place Number Theoretic Transform (Cooley-Tukey)
void ntt(vector<int> &a) {
    int n = (int)a.size();
    int L = 31 - __builtin_clz(n);
    
    static vector<int> rt(2, 1);
    static vector<int> rev;

    if ((int)rev.size() != n) {
        rev.resize(n);
        for (int i = 0; i < n; ++i) {
            rev[i] = (rev[i / 2] | ((i & 1) << L)) / 2;
        }
    }

    if ((int)rt.size() < n) {
        int old_sz = rt.size();
        rt.resize(n);
        for (int k = old_sz; k < n; k *= 2) {
            int z = modpow(ROOT, (MOD - 1) / (2 * k));
            for (int i = k; i < 2 * k; ++i)
                rt[i] = (ll)rt[i / 2] * (i & 1 ? z : 1) % MOD;
        }
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

// Full Unbounded Polynomial Multiplication
vector<int> conv(const vector<int> &a, const vector<int> &b) {
    if (a.empty() || b.empty()) return {};
    
    int s = (int)a.size() + (int)b.size() - 1;
    int B = 32 - __builtin_clz(s);
    int n = 1 << B;
    int inv = modpow(n, MOD - 2, MOD);
    
    vector<int> L(n, 0), R(n, 0), out(n);
    for (int i = 0; i < (int)a.size(); ++i) L[i] = a[i];
    for (int i = 0; i < (int)b.size(); ++i) R[i] = b[i];
    
    ntt(L);
    ntt(R);
    
    for (int i = 0; i < n; i++) {
        out[-i & (n - 1)] = (ll)L[i] * R[i] % MOD * inv % MOD;
    }
    ntt(out);
    
    return {out.begin(), out.begin() + s};
}

// Full Exact Polynomial Power (poly^p) without degree limits
vector<int> poly_pow_full(vector<int> poly, long long p) {
    if (p == 0) return {1}; // (P(x))^0 = 1
    
    vector<int> ans = {1};
    while (p > 0) {
        if (p & 1) ans = conv(ans, poly);
        if (p > 1) poly = conv(poly, poly);
        p >>= 1;
    }
    return ans;
}
/*
    // Full Unbounded Polynomial Multiplication using FFT
vector<long long> multiply(const vector<long long>& a, const vector<long long>& b) {
    if (a.empty() || b.empty()) return {};

    int s = (int)a.size() + (int)b.size() - 1;
    int n = 1;
    while (n < s)
        n <<= 1;

    vector<cd> fa(n), fb(n);
    for (int i = 0; i < (int)a.size(); i++) fa[i] = a[i];
    for (int i = 0; i < (int)b.size(); i++) fb[i] = b[i];

    fft(fa, false);
    fft(fb, false);

    for (int i = 0; i < n; i++)
        fa[i] *= fb[i];

    fft(fa, true);

    vector<long long> result(s);
    for (int i = 0; i < s; i++) {
        result[i] = round(fa[i].real());
    }

    return result;
}

// Full Exact Polynomial Power (poly^p) using FFT without degree limits
vector<long long> poly_pow_full_fft(vector<long long> poly, long long p) {
    if (p == 0) return {1}; // (P(x))^0 = 1

    vector<long long> ans = {1};
    while (p > 0) {
        if (p & 1) ans = multiply(ans, poly);
        if (p > 1) poly = multiply(poly, poly);
        p >>= 1;
    }
    return ans;
}

*/
// =========================================================================
// USAGE EXAMPLE
// =========================================================================
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    // Compute full exact (1 + x)^4
    vector<int> P = {1, 1}; 
    long long exponent = 4;

    vector<int> result = poly_pow_full(P, exponent);

    // Expected Output: 1 4 6 4 1 (size = 5)
    cout << "Full Coefficients of (1 + x)^4: ";
    for (int coeff : result) {
        cout << coeff << " ";
    }
    cout << "\n";

    return 0;
}
