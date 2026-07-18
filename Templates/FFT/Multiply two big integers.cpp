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

string mul_two_big_int(const string &s1, const string &s2) {
    if (s1 == "0" || s2 == "0") return "0";
    
    int n = s1.size(), m = s2.size();
    vector<int> poly1(n), poly2(m);
    
    for (int i = 0; i < n; ++i) poly1[n - i - 1] = s1[i] - '0';
    for (int i = 0; i < m; ++i) poly2[m - i - 1] = s2[i] - '0';

    vector<int> ans = multiply(poly1, poly2); 
    
    long long carry = 0;
    vector<int> digits;
    digits.reserve(ans.size() + 10);
    
    for (size_t i = 0; i < ans.size() || carry > 0; ++i) {
        long long cur = carry;
        if (i < ans.size()) cur += ans[i];
        digits.push_back(cur % 10);
        carry = cur / 10;
    }

    while (digits.size() > 1 && digits.back() == 0) {
        digits.pop_back();
    }

    string final_str = "";
    final_str.reserve(digits.size());
    for (int i = (int)digits.size() - 1; i >= 0; --i) {
        final_str += (char)(digits[i] + '0');
    }

    return final_str;
}

string power_of_big_int(string s, int p) {
    if (p == 0) return "1";
    if (s == "0") return "0";
    
    string ans = "1";
    while (p > 0) {
        if (p & 1) {
            ans = mul_two_big_int(ans, s);
        }
        if (p > 1) { 
            s = mul_two_big_int(s, s);
        }
        p >>= 1;
    }
    return ans;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    // Example Usage
    string base = "999999999";
    int exp = 5;

    string res = power_of_big_int(base, exp);
    cout << base << "^" << exp << " = \n" << res << "\n";

    return 0;
}
