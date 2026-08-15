#include <bits/stdc++.h>
using namespace std;

using ll = long long;

void fwht(vector<ll> &a, bool inv, int f) {
    int sz = a.size();
    for (int len = 1; 2 * len <= sz; len <<= 1) {
        for (int i = 0; i < sz; i += 2 * len) {
            for (int j = 0; j < len; j++) {
                ll x = a[i + j];
                ll y = a[i + j + len];

                if (f == 0) { // Bitwise AND
                    if (!inv)  a[i + j] = y, a[i + j + len] = x + y;
                    else       a[i + j] = y - x, a[i + j + len] = x;
                }
                else if (f == 1) { // Bitwise OR
                    if (!inv)  a[i + j + len] = x + y;
                    else       a[i + j + len] = y - x;
                }
                else { // Bitwise XOR
                    a[i + j] = x + y;
                    a[i + j + len] = x - y;
                }
            }    
        }
    }
}

// Bitwise Convolution: f = 0 (AND), f = 1 (OR), f = 2 (XOR)
// Vector size MUST be a power of 2!
vector<ll> mul(vector<ll> a, vector<ll> b, int f) {
    int sz = a.size();
    fwht(a, false, f); 
    fwht(b, false, f);
    vector<ll> c(sz);
    for (int i = 0; i < sz; ++i) {
        c[i] = a[i] * b[i]; // No Modulo
    }
    fwht(c, true, f);
    
    // Divide by N ONLY for XOR Convolution
    if (f == 2) {
        for (int i = 0; i < sz; ++i) {
            c[i] /= sz; // Direct division since we don't use Mod
        }
    }
    return c;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int max_val = 3;
    int sz = 1;
    while (sz <= max_val) sz <<= 1; // Power of 2

    vector<ll> cntA(sz, 0), cntB(sz, 0);

    cntA[1] = 1; cntA[2] = 1;
    cntB[2] = 1; cntB[3] = 1;

    vector<ll> res_xor = mul(cntA, cntB, 2);

    cout << "XOR Exact Counts:\n";
    for (int i = 0; i < sz; ++i) {
        cout << "val[" << i << "] = " << res_xor[i] << "\n";
    }

    return 0;
}
