#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
#pragma GCC optimize("Ofast")
#define Fast  ios_base::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr);
using namespace std;
using namespace __gnu_pbds;
#define ll long long
#define ld long double
#define ull unsigned long long
#define endl '\n'
#define yes {cout << "YES"<<endl; return ;}
#define no {cout << "NO"<<endl; return ;}
#define mun {cout << "-1\n";return;}
#define all(v) v.begin(), v.end()
#define clr(x, val)	memset((x), (val), sizeof(x))
#define allr(v) v.rbegin(), v.rend()
#define make_unique(v) v.erase(unique(all(v)), v.end());
#define PI acos(-1)
//order_of_key(x): returns number of elements strictly less than x
//find_by_order(k): returns iterator to the k-th smallest element (0-indexed)

ll gcd(ll a, ll b)      {if (b == 0) return a; return gcd(b, a % b);} //O(log min(a, b))
ll lcmm(ll a,ll b)     {return a/gcd(a,b)*b;}
ll summ(ll n)         {return  n * ( n+1)/2;}

ll Abdalraheem () {
    Fast
    return 0;
}
vector<int> dx  = { 1 , -1 , 0 , 0 ,  1 ,  1 , -1 , -1 };
vector<int> dy  = { 0 ,  0 , 1 , -1 ,  1 , -1 , 1 , -1 };
vector<int> dxh = {-2 , -2 , 2 , 2 , 1 , 1 , -1 , -1};
vector<int> dyh = {-1 , 1 , -1 , 1 , -2 , 2 , -2 , 2};

string dir = "DURL";
const int N =101;
const ll M =7490;
const ll oo = 2e18;
const int MOD = 1e9+7;
const ll mod = 24;
#define int ll


template<typename T>
struct SegmentTree {
#define mid ((lx + rx) >> 1)
#define LT (node * 2 + 1)
#define RT (node * 2 + 2)

    int n;
    vector<T> seg;

    SegmentTree(int sz) {
        n = sz;
        seg.assign(4 * n + 1, 0);
    }
    //change it depending on the problem
    T merge(T a, T b) {
        return a + b;
    }

    //build the segment tree from array
    void build(int node, int lx, int rx, vector<T>& a) {

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

    void build(vector<T>& a) {
        build(0, 0, n - 1, a);
    }

    //point update
    //change value at idx to val

    void update(int node, int lx, int rx, int idx, T val) {

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

    void update(int idx, T val) {
        update(0, 0, n - 1, idx, val);
    }

    //query on range [l , r]
    T query(int node, int lx, int rx, int l, int r) {

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

    T query(int l, int r) {
        return query(0, 0, n - 1, l, r);
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
