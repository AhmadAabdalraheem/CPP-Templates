#include <bits/stdc++.h>
using namespace std;

#define int long long

vector<vector<int>> adj;
vector<bool> vis;

void dfs(int u)
{
  vis[u] = true;
  for (auto v : adj[u])
    if (!vis[v]) dfs(v);
}

void solve()
{
  int n, m;
  cin >> n >> m;
  adj.assign(n + 1, {});
  vis.assign(n + 1, false);

  for (int i = 0; i < m; i++)
  {
    int u, v;
    cin >> u >> v;
    adj[u].push_back(v);
    adj[v].push_back(u);
  }

  int cnt = 0;
  for (int i = 1; i <= n; i++)
    if (!vis[i])
      dfs(i), cnt++;

  cout << cnt << '\n';
}

signed main()
{
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int tt;
  cin >> tt;
  while (tt--) solve();
  return 0;
}
