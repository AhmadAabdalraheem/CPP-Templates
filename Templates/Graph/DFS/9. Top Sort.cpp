#include <bits/stdc++.h>
using namespace std;

#define int long long

vector<vector<int>> adj;
vector<bool> vis;
vector<int> order;

void dfs(int u)
{
  vis[u] = true;
  for (auto v : adj[u])
    if (!vis[v]) dfs(v);
  order.push_back(u);
}

void solve()
{
  int n, m;
  cin >> n >> m;
  adj.assign(n + 1, {});
  vis.assign(n + 1, false);
  order.clear();

  for (int i = 0; i < m; i++)
  {
    int u, v;
    cin >> u >> v;
    adj[u].push_back(v);
  }

  for (int i = 1; i <= n; i++)
    if (!vis[i]) dfs(i);

  reverse(order.begin(), order.end());

  for (auto x : order) cout << x << ' ';
  cout << '\n';
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
