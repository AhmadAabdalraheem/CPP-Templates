#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
#define Fast  ios_base::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr);
using namespace std;
#define ll long long

struct SparseTable {
    int n, LOG;
    vector<vector<long long>> st;

    long long merge(long long a, long long b) {
        return a + b; // غيرها: xor / min / max / gcd
    }

    long long NEUTRAL = 0; // مهم: غيره حسب العملية

    void build( vector<long long>& v ) {
        n = v.size();
        LOG = 32 - __builtin_clz(n);
        st.assign(LOG, vector<long long>(n));

        for (int i = 0; i < n; i++)
            st[0][i] = v[i];

        for (int j = 1; j < LOG; j++)
            for (int i = 0; i + (1 << j) <= n; i++)
                st[j][i] = merge(st[j - 1][i],
                                 st[j - 1][i + (1 << (j - 1))]);
    }

    long long query(int l, int r) {
        long long res = NEUTRAL;

        for (int j = LOG - 1; j >= 0; j--) {
            if ((1 << j) <= (r - l + 1)) {
                res = merge(res, st[j][l]);
                l += (1 << j);
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
