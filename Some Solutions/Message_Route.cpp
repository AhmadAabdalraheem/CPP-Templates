#include <bits/stdc++.h>
//#define Abdalraheem  ios_base::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr);
using namespace std;
#define ll long long
#define ld long double
#define ull unsigned long long
#define endl '\n'
#define vi vector<int>
#define vl vector<long long>
#define vii vector<vector<int>>
#define vll vector<vector<long long>>
#define sz size()
#define ed end()
#define bg begin()
#define YES {cout << "YES"<<endl; return;}
#define NO {cout << "NO"<<endl; return;}
#define MUN {cout << "-1\n";return;}
#define all(v) v.begin(), v.end()
#define allr(v) v.rbegin(), v.rend()
#define cin(v) for (auto &_ : v) cin >> _;
#define cout(v) for (auto &_: v) cout << _ << " " ;
#define PI acos(-1)
ll gcd(ll a, ll b)    {if (b == 0) return a; return gcd(b, a % b);} //O(log min(a, b))
ll lcm(ll a,ll b)     {return a/gcd(a,b)*b;}
ll summ(ll n)         {return  n * ( n+1)/2;}

////////////////////////////////////////Sol:////////////////////////////////////////

vi  dx ={0 , 0 , 1 , -1 };
vi dy = {1 , -1 , 0 , 0};

const ll MOD = 1e9+7;
const int nn = 2e6;
int n , m;

vii g(nn);
int d[nn] = {-1};
bool visited[nn];
vi parent(nn, -1);

void solve(int test_cases) {
	cin >> n >> m;
	for (int i = 0 ; i<m ; i++) {
		int u , v ;
		cin >> u >> v ;
		g[u].push_back(v);
		g[v].push_back(u);
	}
	queue<int> q ;
	q.push(1);
	visited[1] = true;
	d[1] = 1;

	while (!q.empty()) {
		int cur = q.front();
		q.pop();
		if (cur == n) {
			break;
		}
		for (auto&it : g[cur]) {
			if (!visited[it]) {
                d[it] = d[cur] + 1;
				parent[it] = cur;
				q.push(it);
				visited[it] = true;
			}
		}
	}
	if (d[n] == -1) {
		cout<<"IMPOSSIBLE"<<endl;
	}
	else {
		vi path;
		int current = n;
		while (current != -1) {
			path.push_back(current);
			current = parent[current];
		}
		reverse(all(path));
		cout << path.sz << endl;
		cout(path);

	}


}



signed main()
{

	//freopen("input.txt", "r", stdin);
	//freopen("output.txt", "w", stdout);
   // Abdalraheem;
    int t= 1;

	//cin >> t;
    for (int i = 1; i <= t; i++)
	solve(i);
}



