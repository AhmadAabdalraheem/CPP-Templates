#include <bits/stdc++.h>
using namespace std;

#define int long long

vector<vector<int>> adj;
vector<int> tin, low;
vector<bool> vis;
int timer;

void dfs(int u, int p)
{
  vis[u] = true;
  tin[u] = low[u] = ++timer;

  for (auto v : adj[u])
  {
    if (v == p) continue;
    if (vis[v])
      low[u] = min(low[u], tin[v]);
    else
    {
      dfs(v, u);
      low[u] = min(low[u], low[v]);
      if (low[v] > tin[u])
        cout << u << " " << v << '\n';
    }
  }
}

void solve()
{
  int n, m;
  cin >> n >> m;
  adj.assign(n + 1, {});
  vis.assign(n + 1, false);
  tin.assign(n + 1, 0);
  low.assign(n + 1, 0);
  timer = 0;

  for (int i = 0; i < m; i++)
  {
    int u, v;
    cin >> u >> v;
    adj[u].push_back(v);
    adj[v].push_back(u);
  }

  for (int i = 1; i <= n; i++)
    if (!vis[i]) dfs(i, -1);
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
