#include <bits/stdc++.h>
using namespace std;

using ll = long long;


int mod = 998244353; 
int root = 3; 

// Standard modular exponentiation helper
int modpow(int b, int e, int m) {
    int ans = 1;
    for (; e; b = (ll)b * b % m, e /= 2)
        if (e & 1) ans = (ll)ans * b % m;
    return ans;
}

// In-place Number Theoretic Transform (Cooley-Tukey)
void ntt(vector<int> &a) {
    int n = (int)a.size();
    int L = 31 - __builtin_clz(n);
    
    vector<int> rt(2, 1); 
    for (int k = 2, s = 2; k < n; k *= 2, s++) { 
        rt.resize(n);
        int z[] = {1, modpow(root, mod >> s, mod)};
        for (int i = k; i < 2 * k; ++i) {
            rt[i] = (ll)rt[i / 2] * z[i & 1] % mod;
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

// Bounded Polynomial Multiplication (Clamped at max_degree)
vector<int> conv(const vector<int> &a, const vector<int> &b, int max_degree) {
    if (a.empty() || b.empty()) return {};
    
    int s = (int)a.size() + (int)b.size() - 1;
    
    // =========================================================================
    // MODIFICATION WARNING:
    // If your problem counts up to index N, pass max_degree = N to maintain O(N log N).
    // =========================================================================
    s = min(s, max_degree + 1); 
    
    int B = 32 - __builtin_clz(s);
    int n = 1 << B;
    int inv = modpow(n, mod - 2, mod);
    
    vector<int> L(n, 0), R(n, 0), out(n);
    for (int i = 0; i < min((int)a.size(), s); ++i) L[i] = a[i];
    for (int i = 0; i < min((int)b.size(), s); ++i) R[i] = b[i];
    
    ntt(L);
    ntt(R);
    
    for (int i = 0; i < n; i++) {
        out[-i & (n - 1)] = (ll)L[i] * R[i] % mod * inv % mod;
    }
    ntt(out);
    
    return {out.begin(), out.begin() + s};
}

// Main Polynomial Exponentiation Function
// O ( N * LOG^2 (N))
//CAN BE USED IN FFT OR NTT
vector<int> poly_pow(vector<int> poly, int p) {
    vector<int> ans{1};
    while (p > 0) {
        if (p & 1) ans = multiply(ans, poly);
        if (p > 1) poly = multiply(poly, poly);
        p >>= 1;
    }
    return ans;
}

// =========================================================================
// USAGE EXAMPLE
// =========================================================================
int main() {
    // Optimize standard I/O operations for performance
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    // Scenario: We want to compute (1 + x)^4 up to degree 4.
    // The coefficients of (1 + x) are represented as vector {1, 1}.
    vector<int> P = {1, 1}; 
    long long exponent = 4;
    int max_degree = 4;

    // Run exponentiation
    vector<int> result = poly_pow(P, exponent, max_degree);

    // Print coefficients of the resulting polynomial:
    // Expected output: 1 4 6 4 1 (corresponds to 1 + 4x + 6x^2 + 4x^3 + x^4)
    cout << "Coefficients of (1 + x)^4: ";
    for (int coeff : result) {
        cout << coeff << " ";
    }
    cout << "\n";

    return 0;
}
