#include <bits/stdc++.h>
#define Fast  ios_base::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr);
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
vector<int> dx = { 1, -1, 0, 0,  1,  1, -1, -1 };
vector<int> dy = { 0,  0, 1,-1,  1, -1,  1, -1 };

const ll N =50;
const ll oo =2e18;
vector<vector<int>>g(N,vector<int>());
bool visited[N]={};
bool path[N]={};
set<ll>st;
int n , k;

bool flag = false;
void dfs(int node) {
    visited[node] = true;
    path[node] = true;

    for (int neighbor : g[node]) {
        if (!visited[neighbor]) {
            dfs(neighbor);
        }
        else if (path[neighbor]) {
            // cycle detected, add all nodes in current path
            for (int i = 1; i <= n; i++) {
                if (path[i]) {
                    st.insert(i);
                }
            }
        }
    }
    path[node] = false; // backtrack
}
void solve(){
    cin>>n>>k;
    for (int i = 0 ; i<k ; i++) {
        int u ,v , wu ,wv;
        cin>>u>>v>>wu>>wv;
        if (wu>wv) {
            g[u].push_back(v);
        }
        else {
            g[v].push_back(u);
        }
    }
    for (int i = 1 ; i<=n ;i++) {
        if (!visited[i]) {
            dfs(i);
        }
    }
    cout<<st.size()<<endl;

}

signed main() {
     //freopen("moocast.in", "r", stdin);
    // freopen("moocast.out", "w", stdout);
    Abdalraheem();
    int t = 1;
    //cin>>t;
    while (t--) {
        solve();
    }
}
