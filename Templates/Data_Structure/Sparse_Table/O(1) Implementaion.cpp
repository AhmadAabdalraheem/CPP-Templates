#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
#define Fast  ios_base::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr);
using namespace std;
#define ll long long

struct SparseTable {
    int n, LOG;
    vector<vector<int>> st;
    vector<int> lg;

    int merge(int a, int b) {
        return min(a, b); // غيرها حسب المسألة (max, gcd...)
    }

    void build(vector<int>& v) {
        n = v.size();
        LOG = 32 - __builtin_clz(n);
        st.assign(LOG, vector<int>(n));
        lg.assign(n + 1, 0);

        for (int i = 2; i <= n; i++)
            lg[i] = lg[i / 2] + 1;

        for (int i = 0; i < n; i++)
            st[0][i] = v[i];

        for (int j = 1; j < LOG; j++) {
            for (int i = 0; i + (1 << j) <= n; i++) {
                st[j][i] = merge(st[j - 1][i],
                                 st[j - 1][i + (1 << (j - 1))]);
            }
        }
    }

    int query(int l, int r) {
        int j = lg[r - l + 1];
        return merge(st[j][l],
                     st[j][r - (1 << j) + 1]);
    }
};


void solve() {
    // I Miss Her
    int n , q;
    cin>>n>>q;
    vector<int>v(n);
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
