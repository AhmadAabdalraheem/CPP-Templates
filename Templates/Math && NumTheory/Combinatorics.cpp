#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

namespace combinatorics
{
    ll MOD;
    vector<ll> fac, inv, finv;

    ll nCr(ll x, ll y)
    {
        if (x < 0 || y > x || y < 0) return 0;
        return (fac[x] * finv[y] % MOD * finv[x - y] % MOD);
    }

    ll nPr(ll x, ll y)
    {
        if (x < 0 || y > x || y < 0) return 0;
        return fac[x] * finv[x - y] % MOD;
    }

    ll power(ll b, ll n)
    {
        b %= MOD;
        ll s = 1;
        while (n)
        {
            if (n % 2 == 1) s = s * b % MOD;
            b = b * b % MOD;
            n /= 2;
        }
        return s;
    }

    void init(int n, ll mod)
    {
        fac.resize(n + 1);
        inv.resize(n + 1);
        finv.resize(n + 1);
        MOD = mod;
        fac[0] = inv[0] = inv[1] = finv[0] = finv[1] = 1;
        for (ll i = 1; i <= n; ++i) fac[i] = fac[i - 1] * i % MOD;
        for (ll i = 2; i <= n; ++i) inv[i] = MOD - MOD / i * inv[MOD % i] % MOD;
        for (ll i = 2; i <= n; ++i) finv[i] = finv[i - 1] * inv[i] % MOD;
    }

    ll mul(ll a, ll b)
    {
        return ((a % MOD) * (b % MOD)) % MOD;
    }

    ll add(ll a, ll b)
    {
        return ((a % MOD) + (b % MOD)) % MOD;
    }

    ll sub(ll a, ll b)
    {
        return (((a - b) % MOD) + MOD) % MOD;
    }

    ll divide(ll a, ll b)
    {
        return mul(a, power(b, MOD - 2));
    }

    ll Inv(int x)
    {
        return power(x, MOD - 2);
    }

    ll catalan(int n)
    {
        return (nCr(2 * n, n) * Inv(n + 1)) % MOD;
    }

    ll StarsAndPars(ll n, ll k)
    {
        return nCr(n + k - 1, k - 1);
    }

    // ==================== NEW ADDITIONS (TIPS & TRICKS) ====================

    // 1. Derangements (D[n]): Number of permutations where no element appears in its original position.
    // Formula: D[n] = (n-1) * (D[n-1] + D[n-2])  OR  D[n] = n! * sum((-1)^i / i!)
    ll derangement(int n) {
        if (n == 0) return 1;
        if (n == 1) return 0;
        ll d1 = 0, d2 = 1, ans = 0;
        for (int i = 2; i <= n; ++i) {
            ans = (i - 1) * (d1 + d2) % MOD;
            d1 = d2;
            d2 = ans;
        }
        return d2;
    }

    // 2. Lucas's Theorem: Computes nCr(n, r) % p when p is a prime and n, r are very large.
    // Time: O(log_p(n))
    ll nCr_Lucas(ll n, ll r, ll p) {
        if (r == 0) return 1;
        ll ni = n % p, ri = r % p;
        if (ri > ni) return 0;
        // Requires fac and finv to be initialized with size >= p
        return (nCr(ni, ri) * nCr_Lucas(n / p, r / p, p)) % p;
    }
};

using namespace combinatorics;

void solve(){
    init(1e6, 1e9 + 7);
}
