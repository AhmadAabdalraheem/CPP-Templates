#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
#define Fast  ios_base::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr);
using namespace std;
#define ll long long

struct SparseTable {
    vector<vector<int>> sp;
    vector<int> lg;
    int n, LOG;

    int merge(int a, int b) {
        return min(a, b); // غيّرها: max / gcd / xor / sum
    }

    void build(vector<int>& v) {
        n = v.size();
        LOG = 32 - __builtin_clz(n);

        sp.assign(n, vector<int>(LOG));
        lg.assign(n + 1, 0);

        for (int i = 2; i <= n; i++)
            lg[i] = lg[i / 2] + 1;

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

    int query(int l, int r) {
        int pow = lg[r - l + 1];
        return merge(
            sp[l][pow],
            sp[r - (1 << pow) + 1][pow]
        );
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
