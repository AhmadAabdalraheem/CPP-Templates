const ll N =1e5+5;
const ll oo =2e18;
int n , m ;
vector<vector<pair<ll,ll>>> g(N);
bool visited[N]={};
vector<ll>dist(N,oo);
vector<ll>parent(N,-1);

void dijkstra(int start) {
            // cost , node  ---------- min-heap
    priority_queue<pair<ll,ll>,vector<pair<ll,ll>> , greater<pair<ll,ll> > >pq;
    pq.push({0,start});
    dist[start] = 0;
    while (!pq.empty()) {
        ll cost = pq.top().first;
        ll node = pq.top().second;
        pq.pop();

        if (cost > dist[node]) {
            continue;
        }
        for(auto [a,b] : g[node])
        {
            if(dist[node] + b < dist[a])
            {
                dist[a] = dist[node] + b;
                parent[a] = node;
                pq.push({dist[a], a});
            }
        }
    }

}
void solve() {
    cin>>n>>m;
    for (int i =0 ; i<m ; i++) {
        ll u , v , w ;
        cin>>u>>v>>w;
        g[u].push_back({v,w});
        g[v].push_back({u,w});
    }
    dijkstra(1);

    if (dist[n] == oo ) {
        cout<<-1<<endl;
        return;
    }
    vector<ll>ans;
    ll cur = n;
    while ( cur != -1) {
        ans.push_back(cur);
        cur = parent[cur];
    }
    reverse(all(ans));
    for (int i = 0;i<ans.size();i++) {
        cout<<ans[i]<<" ";
    }
    cout<<endl;
}
