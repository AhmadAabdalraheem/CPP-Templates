

struct edge {
    int to , a , b;
};
vector<vector<edge>>g;
vector<int>prefixb;
vector<int>ans;

void dfs(int node , int par, int sum_a ,vector<int>& prefixb ) {


    if (node != -1) {
        auto it = upper_bound(all(prefixb) , sum_a) - prefixb.begin();
        ans[node] = it;
    }

    for (auto&it : g[node]) {
        if (node == par) {
            continue;
        }
        int v = it.to;
        if (prefixb.empty()) {
            prefixb.push_back(it.b);
        }
        else {
            prefixb.push_back(it.b + prefixb.back());
        }
        dfs(v,node,sum_a+it.a,prefixb);
        prefixb.pop_back();
    }
}
void solve(){
    int n ;
    cin>>n;
    prefixb.clear();
    g.clear();
    ans.clear();
    g.resize(n+3);
    ans.resize(n+3);

    for (int i = 2 ; i<=n ;i++) {
        int u ,vala , valb;
        cin>>u>>vala>>valb;
        g[u].push_back({i,vala,valb});
    }
    dfs(1,-1,0,prefixb);
    for (int i =2 ;i<=n ;i++) {
        cout<<ans[i]<< ' ';
    }
    cout<<endl;
}
