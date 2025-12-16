#include <bits/stdc++.h>
using namespace std;
#define int long long
#define all(x) (x).begin(), (x).end()
const int MOD = 1e9 + 7;
const int N = 2e5 + 5;
vector<vector<int>> adj;
vector<bool> vis;
vector<int> ans;
void dfs(int u, int v)
{
  ans.push_back(u);
  if (vis[u])
  {
    int x = 0;
    for (int i = 0; i < ans.size(); i++)
    {
      if (ans[i] == u)
      {
        x = i;
        break;
      }
    }
    cout << ans.size() - x << '\n';
    for (int i = x; i < ans.size(); i++)
    {
      cout << ans[i] << ' ';
    }
    exit(0);
  }
  vis[u] = true;
  for (auto i : adj[u])
  {
    if (i != v)
    {
      dfs(i, u);
    }
  }  
  ans.pop_back();
}
signed main()
{
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int n, m;
  cin >> n >> m;
  adj.assign(n + 1, {});
  vis.assign(n + 1, false);
  ans.assign(n + 1, -1);
  for (int i = 0; i < m; i++)
  {
    int u, v;
    cin >> u >> v;
    adj[u].push_back(v);
    adj[v].push_back(u);
  }
  for (int i = 1; i <= n; i++)
  {
    if (!vis[i])
    {
      dfs(i, -1);
    }
  }
  cout << "IMPOSSIBLE";
  return 0;
}
