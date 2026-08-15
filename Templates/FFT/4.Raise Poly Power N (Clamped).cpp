#include <bits/stdc++.h>
using namespace std;

using ll = long long;

// =============================================================================
// TEMPLATE: Fast NTT & Polynomial Exponentiation (Clamped Degree)
// =============================================================================
// TIME COMPLEXITY  : O(N log N log P) for poly_pow
// SPACE COMPLEXITY : O(N)
// MODULO           : 998244353 (Standard NTT Prime)
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

// Bounded Polynomial Multiplication (Clamped at max_degree)
vector<int> conv(const vector<int> &a, const vector<int> &b, int max_degree = 1e9) {
    if (a.empty() || b.empty()) return {};
    
    int s = (int)a.size() + (int)b.size() - 1;
    s = min(s, max_degree + 1); 
    
    int B = 32 - __builtin_clz(s);
    int n = 1 << B;
    int inv = modpow(n, MOD - 2, MOD);
    
    vector<int> L(n, 0), R(n, 0), out(n);
    for (int i = 0; i < min((int)a.size(), s); ++i) L[i] = a[i];
    for (int i = 0; i < min((int)b.size(), s); ++i) R[i] = b[i];
    
    ntt(L);
    ntt(R);
    
    for (int i = 0; i < n; i++) {
        out[-i & (n - 1)] = (ll)L[i] * R[i] % MOD * inv % MOD;
    }
    ntt(out);
    
    return {out.begin(), out.begin() + s};
}

// Polynomial Exponentiation: Computes (poly)^p truncated at max_degree
// Complexity: O(N log N log P)
vector<int> poly_pow(vector<int> poly, long long p, int max_degree) {
    vector<int> ans = {1};
    
    // Trim input polynomial to max_degree
    if ((int)poly.size() > max_degree + 1) {
        poly.resize(max_degree + 1);
    }

    while (p > 0) {
        if (p & 1) ans = conv(ans, poly, max_degree);
        if (p > 1) poly = conv(poly, poly, max_degree);
        p >>= 1;
    }
    return ans;
}
/*
    // Bounded Polynomial Multiplication using FFT (Clamped at max_degree)
vector<long long> multiply(const vector<long long>& a, const vector<long long>& b, int max_degree = 1e9) {
    if (a.empty() || b.empty()) return {};

    int s = (int)a.size() + (int)b.size() - 1;
    s = min(s, max_degree + 1);

    int n = 1;
    while (n < s)
        n <<= 1;

    vector<cd> fa(n, 0), fb(n, 0);
    for (int i = 0; i < min((int)a.size(), s); i++) fa[i] = a[i];
    for (int i = 0; i < min((int)b.size(), s); i++) fb[i] = b[i];

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

// Bounded Polynomial Exponentiation: Computes (poly)^p truncated at max_degree using FFT
// Complexity: O(N log N log P)
vector<long long> poly_pow_bounded_fft(vector<long long> poly, long long p, int max_degree) {
    if (p == 0) return {1}; // (P(x))^0 = 1

    vector<long long> ans = {1};

    // Trim input polynomial to max_degree
    if ((int)poly.size() > max_degree + 1) {
        poly.resize(max_degree + 1);
    }

    while (p > 0) {
        if (p & 1) ans = multiply(ans, poly, max_degree);
        if (p > 1) poly = multiply(poly, poly, max_degree);
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

    // Scenario: Compute (1 + x)^4 up to degree 4.
    // P(x) = 1 + x -> vector {1, 1}
    vector<int> P = {1, 1}; 
    long long exponent = 4;
    int max_degree = 4;

    // Run exponentiation
    vector<int> result = poly_pow(P, exponent, max_degree);

    // Print coefficients of resulting polynomial:
    // Expected output: 1 4 6 4 1
    cout << "Coefficients of (1 + x)^4: ";
    for (int coeff : result) {
        cout << coeff << " ";
    }
    cout << "\n";

    return 0;
}
