#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
#define Fast  ios_base::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr);
using namespace std;
#define ll long long

struct SparseTable {
    int n, LOG;
    vector<vector<long long>> sp;

    long long merge(long long a, long long b) {
        return a + b; // غيرها: min / max / gcd / xor
    }

    long long NEUTRAL = 0;

    void build(vector<long long>& v) {
        n = v.size();
        LOG = 32 - __builtin_clz(n);

        sp.assign(n, vector<long long>(LOG));

        for (int i = 0; i < n; i++)
            sp[i][0] = v[i];

        for (int pow = 1; pow < LOG; pow++) {
            for (int i = 0; i + (1 << pow) <= n; i++) {
                sp[i][pow] = merge(
                    sp[i][pow - 1],
                    sp[i + (1 << (pow - 1))][pow - 1]
                );
            }
        }
    }

    long long query(int l, int r) {
        long long res = NEUTRAL;

        for (int pow = LOG - 1; pow >= 0; pow--) {
            if ((1 << pow) <= r - l + 1) {
                res = merge(res, sp[l][pow]);
                l += (1 << pow);
            }
        }
        return res;
    }
};
void solve() {
    // I Miss Her
    int n , q;
    cin>>n>>q;
    vector<long long>v(n);
    for (int i =0 ; i<n ;i++) {
        cin>>v[i];
    }
    SparseTable st;

    st.build(v);
    while (q--) {
        int l , r;
        cin>>l>>r;
        l-- , r--;
        cout<<st.query(l,r)<<endl;
    }
}
signed main() {
    //freopen("sieve.in", "r", stdin);
    // freopen("where.out", "w", stdout);
    int tt = 1;
    //cin>>tt;
    while (tt--) {
        solve();
    }
}
