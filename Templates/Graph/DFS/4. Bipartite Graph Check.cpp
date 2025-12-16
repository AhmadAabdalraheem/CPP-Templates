#include <bits/stdc++.h>
using namespace std;

#define int long long
#define all(x) (x).begin(), (x).end()

const int MOD = 1e9 + 7;
const int N = 2e5 + 5;

vector<vector<int>> adj;
vector<int> color;
bool ok = true;
int idx = 1;
void dfs(int u)
{
  //cout << color[u] << ' ' << u << '\n';
  for (auto v : adj[u])
  {
    if (color[v] == -1)
    {
      if (!ok)
      {
        return;
      }
      color[v] = !color[u];
      dfs(v);
    }
    else if (color[u] == color[v])
    {
      ok = false;
      return;
    }
  }
}

void solve()
{
  int n, m;
  cin >> n >> m;
  adj.assign(n + 1, {});
  color.assign(n + 1, -1);
  for (int i = 0; i < m; i++)
  {
    int u, v;
    cin >> u >> v;
    adj[u].push_back(v);
    adj[v].push_back(u);
  }
  ok = true;
  for (int i = 1; i <= n; i++)
  {
    if (color[i] == -1)
    {
      dfs(i);
    }
  }
  // if (ok)
  // {
  //   cout << "Scenario #" << idx++ << ":\n";
  //   cout << "No suspicious bugs found!\n";
  // }
  // else
  // {
  //   cout << "Scenario #" << idx++ << ":\n";
  //   cout << "Suspicious bugs found!\n";
  // }
}

signed main()
{
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int tt;
  cin >> tt;
  while (tt--)
  {
    solve();
  }
  return 0;
}
