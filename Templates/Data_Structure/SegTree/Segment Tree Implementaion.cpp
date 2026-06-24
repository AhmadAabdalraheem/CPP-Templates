#include <bits/stdc++.h>
#pragma GCC optimize("Ofast")
#define Fast  ios_base::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr);
using namespace std;
#define ll long long

struct SegmentTree {
#define mid ((lx + rx) >> 1)
#define LT (node * 2 + 1)
#define RT (node * 2 + 2)

    int n;
    vector<int> seg;

    SegmentTree(int sz) {
        n = sz;
        seg.assign(4 * n + 1, 0);
    }
    //change it depending on the problem
    int merge(int a, int b) {
        return a + b;
    }

    //build the segment tree from array
    void build(int node, int lx, int rx, vector<int>& a) {

        //leaf node
        if (lx == rx) {
            seg[node] = a[lx];
            return;
        }
        //build left child
        build(LT, lx, mid, a);

        //build right child
        build(RT, mid + 1, rx, a);

        //after building children merge them
        seg[node] = merge(seg[LT], seg[RT]);
    }

    void build(vector<int>& a) {
        build(0, 0, n - 1, a);
    }

    //point update
    //change value at idx to val

    void update(int node, int lx, int rx, int idx, int val) {

        //reached the wanted index
        if (lx == rx) {
            seg[node] = val;
            return;
        }
        //go left
        if (idx <= mid)
            update(LT, lx, mid, idx, val);

        //go right
        else
            update(RT, mid + 1, rx, idx, val);

        //backtrack and update current node
        seg[node] = merge(seg[LT], seg[RT]);
    }

    void update(int idx, int val) {
        update(0, 0, n - 1, idx, val);
    }

    //query on range [l , r]
    int query(int node, int lx, int rx, int l, int r) {

        //no overlap
        if (rx < l || lx > r)
            return 0;

        //complete overlap
        if (lx >= l && rx <= r)
            return seg[node];


        //partial overlap
        return merge(
            query(LT, lx, mid, l, r),
            query(RT, mid + 1, rx, l, r)
        );
    }

    int query(int l, int r) {
        return query(0, 0, n - 1, l, r);
    }

    void print(int node, int lx, int rx){

        cout << "node = " << node
             << " range = [" << lx << "," << rx << "] "
             << "value = " << seg[node] << endl;

        if(lx == rx)
            return;

        print(LT, lx, mid);
        print(RT, mid + 1, rx);
    }
#undef mid
#undef LT
#undef RT
};

void solve(){

    int n, q;
    cin >> n >> q;

    vector<int> a(n);

    for (auto &i : a)
        cin >> i;

    SegmentTree<int> seg(n);

    //build segment tree from array
    seg.build(a);

    while (q--) {

        int op;
        cin >> op;
        if (op == 1) {

            int idx, val;
            cin >> idx >> val;

            seg.update(idx, val);
        }

        else {
            int l, r;
            cin >> l >> r;

            cout << seg.query(l, r - 1) << endl;
        }
    }
}


signed main() {
    Abdalraheem();
    //freopen("sieve.in", "r", stdin);
    // freopen("where.out", "w", stdout);
    int tt = 1;
   // cin>>tt;
    while (tt--) {
        solve();
    }
}
