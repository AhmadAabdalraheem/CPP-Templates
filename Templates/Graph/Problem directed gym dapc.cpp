/*
  Problem Summary: G. Genealogy Gumbo

Context:
- You are given N genealogical relationships specifying parent-child pairs in the format "A, son of B" (meaning B is the parent of A).

Goal:
- Determine whether it is possible for all named individuals to descend from a single common ancestor (a single root).

Key Graph Translation & Rules:
- The input forms a directed graph where each relationship "A, son of B" creates a directed edge: B -> A.
- You must check if there exists at least one source node (candidate root) that can reach every other node in the graph via directed paths.

Output:
- Print "possible" if a common ancestor exists who can reach all nodes, otherwise print "impossible".
*/




#include <bits/stdc++.h>
using namespace std;

unordered_map<string, int> name_to_id;
vector<vector<int>> g;
vector<bool> visited;


int get_id(const string& name) {
    if (name_to_id.find(name) == name_to_id.end()) {
        int id = name_to_id.size();
        name_to_id[name] = id;
    }
    return name_to_id[name];
}


void dfs(int node , int& count) {
    visited[node] = true;
    count++;

    for (auto&it :g[node]) {
        if (!visited[it]) {
            dfs(it,count);
        }
    }
}
void solve() {
    int n ;
    cin>>n;
    vector<pair<int, int>> edges;

    cin.ignore();
    string s;


    for (int i = 0; i < n; i++) {
        getline(cin, s);
        int comma = s.find(',');

        string son = s.substr(0, comma);
        string father = s.substr(comma + 9);

        int u = get_id(father);
        int v = get_id(son);
        edges.push_back({u, v});

    }
    int total = name_to_id.size();
    g.resize(total);

    for (auto&[a,b]: edges) {
        g[a].push_back(b);
    }

    visited.assign(total,false);
    int root= 0 ;
    int check = 0;

    for (int i =0 ;i < total ; i++) {
        if (!visited[i]) {
            root = i;
            dfs(i,check);
        }
    }
    visited.assign(total,false);

    int ans = 0;
    dfs(root,ans);

    if (ans == check) {
        cout<<"possible";
    }
    else {
        cout<<"impossible";
    }

}

signed main() {
    Abdalraheem();
    //freopen("output.txt", "w", stdout);
    int tt = 1;

    //cin>>tt;
    while (tt--) {
        solve();
    }
}
