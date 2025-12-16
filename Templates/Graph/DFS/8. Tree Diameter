#include <bits/stdc++.h>
using namespace std;
#define int long long
#define all(x) (x).begin(), (x).end()
const int MOD = 1e9 + 7;
const int N = 2e5 + 5;
vector<vector<int>> adj;
int bst, far;
void dfs(int u, int p, int depth)
{
  if (depth > bst)
  {
    bst = depth, far = u;
  }
  for (auto v : adj[u])
  {
    if (v != p)
    {
      dfs(v, u, depth + 1);
    }
  }
}
signed main()
{
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int n;
  cin >> n;
  adj.assign(n + 1, {});
  for (int i = 0; i < n - 1; i++)
  {
    int u, v;
    cin >> u >> v;
    adj[u].push_back(v);
    adj[v].push_back(u);
  }
  bst = -1;
  dfs(1, -1, 0);
  int x = far;
  dfs(x, -1, 0);
  cout << bst;
  return 0;
}
