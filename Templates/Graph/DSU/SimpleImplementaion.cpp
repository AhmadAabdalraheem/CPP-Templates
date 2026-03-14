#include <bits/stdc++.h>
using namespace std;
#define ll long long
#define vi vector<int>
#define vl vector<long long>

;
ll n ;
struct DSU {
    vector<int>parent,size_of_component;
    void init(int nn ) {
        parent.resize(nn+1);
        size_of_component.resize(nn+1);
        for (int i =1 ;i<=nn;i++) {
            parent[i]=i;
            size_of_component[i] =1 ;
        }
    }
    int find_root(int node) {
            if (node == parent[node]) {
                return node;
            }
        return parent[node] = find_root(parent[node]);
    }
    bool merge(int u , int v) { // q log n

        int root_u = find_root(u);
        int root_v = find_root(v);

        if (root_u == root_v) {
            return false ;
        }

        // merge smaller comp to bigger comp
        if (size_of_component[root_u]> size_of_component[root_v] ) {
            swap(root_u,root_v);
        }
        parent[root_u]= root_v;


        size_of_component[root_v] += size_of_component[root_u];
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
