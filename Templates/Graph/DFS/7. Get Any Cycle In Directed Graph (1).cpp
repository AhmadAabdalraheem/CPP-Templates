#include <bits/stdc++.h>
using namespace std;
#define int long long
#define all(x) (x).begin(), (x).end()
const int MOD = 1e9 + 7;
const int N = 2e5 + 5;
vector<vector<int>> adj;
vector<short> vis;
vector<int> ans;
void dfs(int u, int p)
{
  ans.push_back(u);
  if (vis[u] == 1)
  {
    int x = 0;
    for (int j = 0; j < ans.size(); j++)
    {
      if (ans[j] == u)
      {
        x = j;
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
  vis[u] = 1;
  for (auto v : adj[u])
  {
    if (vis[v] != 2)
    {
      dfs(v, u);
    }
  }
  vis[u] = 2;
  ans.pop_back();
}
signed main()
{
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int n, m;
  cin >> n >> m;
  adj.assign(n + 1, vector<int>());
  vis.assign(n + 1, false), ans.assign(n + 1, -1);
  for (int i = 0; i < m; i++)
  {
    int u, v;
    cin >> u >> v;
    adj[u].push_back(v);
    // adj[v].push_back(u);
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
