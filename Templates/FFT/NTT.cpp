#include <bits/stdc++.h>
using namespace std;

using ll = long long;

// Global settings initialized dynamically or manually
ll mod = 998244353; 
int root = 3; 

ll modpow(ll b, ll e) {
    ll ans = 1;
    for (; e; b = b * b % mod, e /= 2)
        if (e & 1) ans = ans * b % mod;
    return ans;
}

int modpow(int b, int e, int m) {
    int ans = 1;
    for (; e; b = (ll)b * b % m, e /= 2)
        if (e & 1) ans = (ll)ans * b % m;
    return ans;
}

// Automatically finds the primitive root for the active global 'mod'
int generator() {
    vector<int> fact;
    int phi = mod - 1, n = phi;
    for (int i = 2; i * i <= n; ++i) {
        if (n % i == 0) {
            fact.push_back(i);
            while (n % i == 0) n /= i;
        }
    }
    if (n > 1) fact.push_back(n);

    for (int res = 2; res <= mod; ++res) {
        bool ok = true;
        for (size_t i = 0; i < fact.size() && ok; ++i) {
            ok &= modpow(res, phi / fact[i]) != 1;
        }
        if (ok) return res;
    }
    return -1;
}

// In-place NTT loop
void ntt(vector<int> &a) {
    int n = (int)a.size(), L = 31 - __builtin_clz(n);
    
    vector<int> rt(2, 1); 
    for (int k = 2, s = 2; k < n; k *= 2, s++) { 
        rt.resize(n);
        
        int z[] = {1, modpow(root, mod >> s, mod)};
        
        for (int i = k; i < 2 * k; ++i) 
            rt[i] = (ll)rt[i / 2] * z[i & 1] % mod;
    }
    vector<int> rev(n);
    for (int i = 0; i < n; ++i) {
        rev[i] = (rev[i / 2] | (i & 1) << L) / 2;
    }
    for (int i = 0; i < n; ++i){
        if (i < rev[i]) swap(a[i], a[rev[i]]);
    }
    for (int k = 1; k < n; k *= 2) {
        for (int i = 0; i < n; i += 2 * k) {
            for (int j = 0; j < k; ++j) {
                int z = (ll)rt[j + k] * a[i + j + k] % mod, &ai = a[i + j];
                
                a[i + j + k] = ai - z + (z > ai ? mod : 0);
                
                ai += (ai + z >= mod ? z - mod : z);
            }
        }
    }
}

// Single-modulus Convolution
vector<int> conv(const vector<int> &a, const vector<int> &b) {
    if (a.empty() || b.empty()) return {};
    
    int s = (int)a.size() + (int)b.size() - 1, B = 32 - __builtin_clz(s), n = 1 << B;
    
    int inv = modpow(n, mod - 2, mod);
    
    vector<int> L(a), R(b), out(n);
    
    L.resize(n), R.resize(n);
    
    ntt(L), ntt(R);
    
    for (int i = 0; i < n; ++i) out[-i & (n - 1)] = (ll)L[i] * R[i] % mod * inv % mod;
    
    ntt(out);
    
    return {out.begin(), out.begin() + s};
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    // Dynamic root setup at runtime based on mod configuration
    mod = 998244353;
    root = generator(); 

    vector<int> poly_A = {1, 2, 3};
    vector<int> poly_B = {4, 5, 6};

    vector<int> result = conv(poly_A, poly_B);

    for (int x : result) cout << x << " ";
    cout << "\n";

    return 0;
}
