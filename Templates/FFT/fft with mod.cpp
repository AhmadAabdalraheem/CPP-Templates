#include <bits/stdc++.h>
using namespace std;

#define fast_io ios_base::sync_with_stdio(false); cin.tie(NULL);
using ll = long long;
using vi = vector<int>;

// if 1i is not defined add using namespace std::complex_literals;
// polynomial multiplication under an arbitrary modulo (often referred to as Any-Mod FFT

/*
 * ============================================================================
 * 📦 TEMPLATE OVERVIEW: Any-Mod FFT (KACTL convMod)
 * ============================================================================
 *
 * 🎯 USAGE:
 *  - Arbitrary Modulo Convolution: Multiplies two polynomials (integer arrays)
 *    modulo M, where M does NOT need to be an NTT-friendly prime (e.g., works
 *    for M = 10^9 + 7, composite numbers, or non-standard primes).
 *  - High-performance counting, DP optimizations, and generating functions
 *    under arbitrary modular arithmetic.
 *
 * ⚠️ LIMITATIONS & CAVEATS:
 *  - Array Size & Modulo Bound: Uses double precision. Safe for polynomial
 *    sizes up to N ≈ 10^5 with modulus M ≈ 10^9. Larger sizes/moduli may 
 *    suffer from double-precision floating-point rounding errors.
 *  - Constant Factor: Runs 4 FFT transforms per call. Roughly 2x-3x slower 
 *    than standard NTT on 998244353.
 *  - Input Range: Elements in input vectors a and b must be in [0, M - 1].
 *  - Time Complexity: O(N log N) time, O(N) space.
 * ============================================================================
 */

// --- KACTL Arbitrary Modulo FFT ---
typedef complex<double> C;
typedef vector<double> vd;

void fft(vector<C>& a) {
    int n = a.size();
    int L = 31 - __builtin_clz(n);
    static vector<complex<long double>> R(2, 1);
    static vector<C> rt(2, 1);
    for (static int k = 2; k < n; k *= 2) {
        R.resize(n); rt.resize(n);
        auto x = polar(1.0L, acos(-1.0L) / k);
        for (int i = k; i < 2 * k; i++) {
            rt[i] = R[i] = i & 1 ? R[i / 2] * x : R[i / 2];
        }
    }
    vi rev(n);
    for (int i = 0; i < n; i++) {
        rev[i] = (rev[i / 2] | (i & 1) << L) / 2;
    }
    for (int i = 0; i < n; i++) {
        if (i < rev[i]) swap(a[i], a[rev[i]]);
    }
    for (int k = 1; k < n; k *= 2) {
        for (int i = 0; i < n; i += 2 * k) {
            for (int j = 0; j < k; j++) {
                auto x = (double *)&rt[j + k], y = (double *)&a[i + j + k];
                C z(x[0] * y[0] - x[1] * y[1], x[0] * y[1] + x[1] * y[0]);
                a[i + j + k] = a[i + j] - z;
                a[i + j] += z;
            }
        }
    }
}
 
template<int M> 
vi convMod(const vi &a, const vi &b) {
    if (a.empty() || b.empty()) return {};
    vi res((int)a.size() + (int)b.size() - 1);
    int B = 32 - __builtin_clz((int)res.size()), n = 1 << B, cut = int(sqrt(M));
    vector<C> L(n), R(n), outs(n), outl(n);
    for (int i = 0; i < (int)a.size(); i++) {
        L[i] = C((int)a[i] / cut, (int)a[i] % cut);
    }
    for (int i = 0; i < (int)b.size(); i++) {
        R[i] = C((int)b[i] / cut, (int)b[i] % cut);
    }
    fft(L), fft(R);
    for (int i = 0; i < n; i++) {
        int j = -i & (n - 1);
        outl[j] = (L[i] + conj(L[j])) * R[i] / (2.0 * n);
        outs[j] = (L[i] - conj(L[j])) * R[i] / (2.0 * n) / 1i;
    }
    fft(outl), fft(outs);
    for (int i = 0; i < (int)res.size(); i++) {
        ll av = ll(real(outl[i]) + .5), cv = ll(imag(outs[i]) + .5);
        ll bv = ll(imag(outl[i]) + .5) + ll(real(outs[i]) + .5);
        res[i] = ((av % M * cut + bv) % M * cut + cv) % M;
    }
    return res;
}

int main() {
    fast_io;
    
    const int MOD = 1e9 + 7;
    vi A = {1, 2, 3};
    vi B = {3, 4};

    // Expected Output: 3 10 17 12
    vi C = convMod<MOD>(A, B);
    
    for (int x : C) {
        cout << x << " ";
    }
    cout << "\n";
    
    return 0;
}
