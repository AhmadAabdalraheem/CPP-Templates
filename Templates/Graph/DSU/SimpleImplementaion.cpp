#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
#define Fast  ios_base::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr);
using namespace __gnu_pbds;
using namespace std;
#define ll long long
#define ld long double
#define ull unsigned long long
#define endl '\n'
#define vi vector<int>
#define vl vector<long long>
#define vii vector<vector<int>>
#define vll vector<vector<ll>>
#define yes {cout << "YES"<<endl; return ;}
#define no {cout << "NO"<<endl; return ;}
#define mun {cout << "-1\n";return;}
#define all(v) v.begin(), v.end()
#define clr(x, val)	memset((x), (val), sizeof(x))
#define allr(v) v.rbegin(), v.rend()
#define cin(v) for (auto &_ : v) cin >> _;
#define cout(v) for (auto &_: v) cout << _ << " " ;
#define make_unique(v) v.erase(unique(all(v)), v.end());
#define PI acos(-1)
//order_of_key(x): returns number of elements strictly less than x
//find_by_order(k): returns iterator to the k-th smallest element (0-indexed)

ll gcd(ll a, ll b)      {if (b == 0) return a; return gcd(b, a % b);} //O(log min(a, b))
ll lcmm(ll a,ll b)     {return a/gcd(a,b)*b;}
ll summ(ll n)         {return  n * ( n+1)/2;}
typedef tree<int, null_type, less_equal<int>, rb_tree_tag,
            tree_order_statistics_node_update>
   ordered_multiset;

ll Abdalraheem () {
    Fast
    return 0;
}
ll Mex(vector<ll>&s)
{
    set<ll>st(s.begin(),s.end());
    ll idx=0;
    for(auto it:st)
    {
        if(it!=idx)return idx;
        idx++;
    }
    return idx;
}

const ll N =1e5+2;
const ll oo =2e18;
ll n ;
struct DSU {
    vector<int>parent;
    void init(int nn ) {
        parent.resize(nn+1);
        for (int i =1 ;i<=nn;i++) {
            parent[i]=i;
        }
    }
    int find_root(int node) {
            if (node == parent[node]) {
                return node;
            }
        return find_root(  parent[node] );
    }
    bool merge(int u , int v) {

        int root_u = find_root(u);
        int root_v = find_root(v);
        if (root_u == root_v) {
            return false ;
        }
        parent[root_u]= root_v;
        return true;
    }
};
void solve() {
    int q;
    cin>>n>>q;
    DSU d;
    d.init(n+1);

    while (q--) {
        int op , u , v;
        cin>>op>>u>>v;
        if (op == 1) {
            d.merge(u,v);
        }
        else {
            if (d.find_root(u) == d.find_root(v)) {
                cout<<"Connected";
            }
            else {
                cout<<"Not Connected";
            }
        }
    }

}

signed main() {
   // Abdalraheem();
    // freopen("where.in", "r", stdin);
    // freopen("where.out", "w", stdout);
    int t = 1;
   // cin>>t;
    while (t--) {
        solve();
    }
}
