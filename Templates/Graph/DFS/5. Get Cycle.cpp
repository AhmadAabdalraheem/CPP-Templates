#include <bits/stdc++.h>
using namespace std;

#define int long long
#define all(x) (x).begin(), (x).end()

vector<vector<int>> adj;
vector<int> parent, cycle;
vector<bool> vis, inPath;

bool dfs(int u, int p)
{
  vis[u] = true;
  inPath[u] = true;
  parent[u] = p;

  for (auto v : adj[u])
  {
    if (v == p) continue;

    if (!vis[v])
    {
      if (dfs(v, u)) return true;
    }
    else if (inPath[v])
    {
      int cur = u;
      cycle.push_back(v);
      while (cur != v)
      {
        cycle.push_back(cur);
        cur = parent[cur];
      }
      cycle.push_back(v);
      reverse(all(cycle));
      return true;
    }
  }

  inPath[u] = false;
  return false;
}

signed main()
{
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int n, m;
  cin >> n >> m;

  adj.assign(n + 1, {});
  parent.assign(n + 1, -1);
  vis.assign(n + 1, false);
  inPath.assign(n + 1, false);

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
      if (dfs(i, -1))
        break;
    }
  }

  if (cycle.empty())
  {
    cout << "IMPOSSIBLE\n";
  }
  else
  {
    cout << cycle.size() << '\n';
    for (auto x : cycle) cout << x << ' ';
    cout << '\n';
  }

  return 0;
}
