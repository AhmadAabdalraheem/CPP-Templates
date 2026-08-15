#include <bits/stdc++.h>
using namespace std;

    int n ;

/*
  Your task is to determine for each node the maximum distance to another node.

    Input:

5
1 2
1 3
3 4
3 5
Output:

2 3 2 3 3
*/
 
int bfs(int node ,vector<int>& dist,vector<vector<int>>& g ) {
 
    dist.assign(n+1,-1);
 
    queue<int> q;
    q.push(node);
    int far = node;
    dist[node] = 0;
 
    while (!q.empty()) {
        int cur = q.front();
        q.pop();
 
        for (auto it : g[cur]) {
            if (dist[it] == -1) {
                dist[it] = dist[cur]+1;
                q.push(it);
 
                if (dist[far] < dist[it]) {
                    far = it;
                }
            }
        }
    }
    return far;
}
 
 
 
void solve() {
    //Did it reach her ??
    vector<vector<int>>g;
 
    vector<int>dista;
    vector<int>distb;
 
    cin>>n;
    g.resize(n+1,vector<int>());
    for (int i =1 ; i<=n-1; i++) {
        int u , v;
        cin>>u>>v;
        g[u].push_back(v);
        g[v].push_back(u);
    }
 
    int fara = bfs(1,dista,g);
 
     fara = bfs(fara,dista,g);
     int farb = bfs(fara,distb,g);
 
     for (int i =1 ;i<=n;i++) {
         cout<<max(dista[i],distb[i])<<' ';
     }
}
 
