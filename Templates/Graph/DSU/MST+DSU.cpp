#include <bits/stdc++.h>

struct edge {
    int u ,  v ,  w;
};
bool cmp(const edge& x, const edge& y) {
    return x.w < y.w;
}
vector<edge>v;

struct DSU {
   vector<int>parent,sizes;
    vector<int>mini,maxi;
    vector<int> component_edges;
    int num_components;

    void init(int nn ) {
        parent.resize(nn+1);
        sizes.resize(nn+1);
        mini.resize(nn+1);
        maxi.resize(nn+1);
        component_edges.resize(nn+1);

        num_components = nn; // nn must be equal no number of nodes
        for (int i =1 ; i<=nn;i++) {
            parent[i] = i;
            sizes[i]=1;
            component_edges[i]=0;
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
    bool is_same_set(int u, int v) {
        return find_root(u) == find_root(v);
    }
    int count_components() { 
        return num_components; 
    }
    bool merge(int u , int v) {
 
        int root_u = find_root(u);
        int root_v = find_root(v);
 
        if (root_u == root_v) {
            component_edges[root_u]++;
            return false ;
        }
 
        // merge smaller comp to bigger comp
        if (sizes[root_u] > sizes[root_v] ) {
            swap(root_u,root_v);
        }
        parent[root_u]= root_v;
        mini[root_v] = min(mini[root_v], mini[root_u]);
        maxi[root_v] = max(maxi[root_v], maxi[root_u]);
        
        component_edges[root_v] += component_edges[root_u] + 1;
 
        sizes[root_v] += sizes[root_u];
        num_components--;
        return true;
    }

};
    long long kruskal(int n, vector<edge>& edge_list) {  
    sort(edge_list.begin(), edge_list.end(), cmp); 
    DSU dsu;
    dsu.init(n);
    
    long long total_weight = 0;
    for (auto [u, v, w] : edge_list) {
        if (dsu.merge(u, v)) {
            total_weight += w;
        }
    }
    return total_weight;
    }

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
