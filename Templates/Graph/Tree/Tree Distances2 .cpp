#include <bits/stdc++.h>
#define ll long long

/* Your task is to determine for each node the sum of the distances from the node to all other nodes.
Input:

5
1 2
1 3
3 4
3 5
Output:

6 9 5 8 8
*/
vector<vector<int>>g;
vector<int>ans;
vector<int> subtree, depth;
            //dp

    int n ;
void dfs1(int node ,int par) {
    subtree[node] = 1;

    for (int it : g[node]) {
        if (it == par) {
            continue;
        }
        depth[it] = depth[node] + 1;
        dfs1(it, node);
        subtree[node] += subtree[it];
    }
}
void dfs2(int node , int parent) {

    for (auto it:g[node]) {
        if (it != parent) {

            ans[it]=ans[node] +n - 2*subtree[it];
            dfs2(it,node);

        }
    }
}
void solve() {
    //Did it reach her ??
    cin>>n;
    g.resize(n+1,vector<int>());
    depth.assign(n+1,0);
    subtree.assign(n+1,0);
    ans.assign(n+1,0);
    for (int i =1 ; i<=n-1;i++) {
        int u , v;
        cin>>u>>v;
        g[u].push_back(v);
        g[v].push_back(u);
    }
    dfs1(1,-1);
    int x = 0;
    for (int i = 1 ; i<=n;i++) {
        x+=depth[i];
    }

    ans[1] = x;
    dfs2(1,-1);



    for (int i = 1; i<=n;i++) {
        cout<<ans[i]<<' ';
    }


}


signed main() {
    Abdalraheem();
   // freopen("clocktree.in", "r", stdin);
    // freopen("clocktree.out", "w", stdout);
    int tt = 1;
    //cin>>tt;
    while (tt--) {
        solve();
    }
}
