#include <bits/stdc++.h>

struct edge {
    int u ,  v ,  w;
};
bool cmp(edge x , edge y ) {
    return x.w <y.w;
}
vector<edge>v;

struct DSU {
    vector<int>parent,sizes;
    vector<int>mini,maxi;

    void init(int nn ) {
        parent.resize(nn+1);
        sizes.resize(nn+1);
        mini.resize(nn+1);
        maxi.resize(nn+1);

        iota(all(parent),0);
        fill(all(sizes),1);
        iota(all(mini),0);
        iota(all(maxi),0);
    }
    int find_root(int node) {
        if (node == parent[node]) {
            return node;
        }
        return parent[node] = find_root(parent[node]);
    }
    int get_size(int u){
        return sizes[find_root(u)];
    }
    bool merge(int u , int v) {

        int root_u = find_root(u);
        int root_v = find_root(v);

        if (root_u == root_v) {
            return false ;
        }

        // merge smaller comp to bigger comp
        if (sizes[root_u] > sizes[root_v] ) {
            swap(root_u,root_v);
        }
        parent[root_u]= root_v;
        mini[root_v] = min(mini[root_v], mini[root_u]);
        maxi[root_v] = max(maxi[root_v], maxi[root_u]);

        sizes[root_v] += sizes[root_u];
        return true;
    }


    int kruskal(vector <edge> edges) {  //MST
        sort(all(edges),cmp);
        int total= 0;

        for (auto [u,v,w] : edges) {
            if (merge(u,v)) {
                total += w;
            }
        }
        return total;
    }

};

void solve() {
    int t;
    cin>>t;
    while (t--) {

        int n ;
        cin>>n;
        DSU d;
        d.init(n+10);

        for (int i =1 ; i<=n;i++) {
            string name ;
            int k ;
            cin>>name>>k;

            for (int j =0 ; j<k;j++) {
                int u , c;
                cin>>u>>c;
                v.push_back({i,u,c});
            }
        }
        cout<<d.kruskal(v)<<endl;
        v.clear();
    }

}

signed main() {
    Abdalraheem();
    // freopen("where.in", "r", stdin);
    // freopen("where.out", "w", stdout);
    int tt = 1;
    //cin>>tt;
    while (tt--) {
        solve();
    }
}
