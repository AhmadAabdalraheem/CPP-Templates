#include <bits/stdc++.h>
using namespace std;

using ll = long long;
const int mod = 998244353;

int add(int a, int b) {
    return (a + b) % mod;
}

int sub(int a, int b) {
    return (a - b + mod) % mod;
}

ll poww(ll a, ll b) {
    ll ret = 1;
    while (b) {
        if (b & 1) ret = ret * a % mod;
        a = a * a % mod;
        b >>= 1;
    }
    return ret;
}

void fwht(vector<int> &a, int inv, int f) {
    int sz = a.size();
    for (int len = 1; 2 * len <= sz; len <<= 1) {
        for (int i = 0; i < sz; i += 2 * len) {
            for (int j = 0; j < len; j++) {
                int x = a[i + j];
                int y = a[i + j + len];

                if (f == 0) { // Bitwise AND
                    if (!inv)  a[i + j] = y, a[i + j + len] = add(x, y);
                    else       a[i + j] = sub(y, x), a[i + j + len] = x;
                }
                else if (f == 1) { // Bitwise OR
                    if (!inv)  a[i + j + len] = add(x, y);
                    else       a[i + j + len] = sub(y, x);
                }
                else { // Bitwise XOR
                    a[i + j] = add(x, y);
                    a[i + j + len] = sub(x, y);
                }
            }    
        }
    }
}

// Bitwise Convolution: f = 0 (AND), f = 1 (OR), f = 2 (XOR)
// NOTE: Vector sizes MUST be a power of 2!
vector<int> mul(vector<int> a, vector<int> b, int f) {
    int sz = a.size();
    fwht(a, 0, f); 
    fwht(b, 0, f);
    vector<int> c(sz);
    for (int i = 0; i < sz; ++i) {
        c[i] = 1ll * a[i] * b[i] % mod;
    }
    fwht(c, 1, f);
    
    // FIX: Divide by N ONLY for XOR Convolution (f == 2)
    if (f == 2) {
        int sz_inv = poww(sz, mod - 2);
        for (int i = 0; i < sz; ++i) {
            c[i] = 1ll * c[i] * sz_inv % mod;
        }
    }
    return c;
}

// =========================================================================
// USAGE EXAMPLE IN MAIN
// =========================================================================
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    // Array A = {1, 2}
    // Array B = {2, 3}
    // We want to count pairs (A[i], B[j]) for XOR, OR, AND operations.

    int max_val = 3; // Maximum value in input arrays
    
    // Size MUST be the smallest power of 2 strictly greater than max_val
    int sz = 1;
    while (sz <= max_val) sz <<= 1; // sz = 4 (power of 2)

    vector<int> cntA(sz, 0), cntB(sz, 0);

    // Build frequency arrays
    cntA[1] = 1; cntA[2] = 1; // Array A has 1 and 2
    cntB[2] = 1; cntB[3] = 1; // Array B has 2 and 3

    // -------------------------------------------------------------------------
    // 1. Bitwise XOR Convolution (f = 2)
    // -------------------------------------------------------------------------
    // Pairs (x, y): (1^2=3), (1^3=2), (2^2=0), (2^3=1)
    // Expected output counts: idx 0:1, idx 1:1, idx 2:1, idx 3:1
    vector<int> res_xor = mul(cntA, cntB, 2);
    cout << "XOR Result Counts: ";
    for (int i = 0; i < sz; ++i) cout << "val[" << i << "]=" << res_xor[i] << " ";
    cout << "\n";

    // -------------------------------------------------------------------------
    // 2. Bitwise OR Convolution (f = 1)
    // -------------------------------------------------------------------------
    // Pairs (x, y): (1|2=3), (1|3=3), (2|2=2), (2|3=3)
    // Expected output counts: idx 2:1, idx 3:3
    vector<int> res_or = mul(cntA, cntB, 1);
    cout << "OR  Result Counts: ";
    for (int i = 0; i < sz; ++i) cout << "val[" << i << "]=" << res_or[i] << " ";
    cout << "\n";

    // -------------------------------------------------------------------------
    // 3. Bitwise AND Convolution (f = 0)
    // -------------------------------------------------------------------------
    // Pairs (x, y): (1&2=0), (1&3=1), (2&2=2), (2&3=2)
    // Expected output counts: idx 0:1, idx 1:1, idx 2:2
    vector<int> res_and = mul(cntA, cntB, 0);
    cout << "AND Result Counts: ";
    for (int i = 0; i < sz; ++i) cout << "val[" << i << "]=" << res_and[i] << " ";
    cout << "\n";

    return 0;
}
