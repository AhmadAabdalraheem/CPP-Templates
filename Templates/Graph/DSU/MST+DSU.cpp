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
    vector<int>edges;
    void init(int nn ) {
        parent.resize(nn+1);
        sizes.resize(nn+1);
        mini.resize(nn+1);
        maxi.resize(nn+1);
        edges.resize(nn+1);
        for (int i =1 ; i<=nn;i++) {
            parent[i] = i;
            sizes[i]=1;
            edges[i]=0;
            maxi[i]=i;
            mini[i]=i;
        }
 
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
        
        edges[root_v] += edges[root_u];;
 
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
   
    int n , m ;
    cin>>n>>m;
    DSU d ;
    d.init(n+5);
    for (int i =0;i<m;i++) {
        int u ,v;
        cin>>u>>v;
        d.merge(u,v);
        int x = d.find_root(v);
        d.edges[x]++;
    }
 
    for (int i =1 ;i<=n;i++) {
        if (d.find_root(i) == i ) {
            int k = d.sizes[i];
            if (d.edges[i] != k*(k-1) /2) {
                no;
            }
        }
    }
    yes;
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
