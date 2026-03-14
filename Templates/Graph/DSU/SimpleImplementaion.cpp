#include <bits/stdc++.h>
using namespace std;
#define ll long long
#define vi vector<int>
#define vl vector<long long>

;
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
            return parent[node] = find_root(parent[node]);
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
    int t = 1;
   // cin>>t;
    while (t--) {
        solve();
    }
}
