#include <bits/stdc++.h>
#define Abdelraheem  ios_base::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr);
using namespace std;
#define ll long long
#define ld long double
#define endl '\n'
#define vi vector<int>
#define vl vector<long long >
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
#define setp(n) cout << fixed << setprecision(n)

ll gcd(ll a, ll b)
{
    if (b == 0) return a;
    return gcd(b, a % b);
}
ll lcm(ll a,ll b)
{
    return a/gcd(a,b)*b;
}
ll sum(ll n)
{
    return  n * ( n+1)/2;
}

vector<long long> divisors(long long n)
{
	vector<ll>arr;
	for (long long i = 1; i * i <= n; i++)
	{
		if (n % i == 0)
		{
			arr.push_back(i);
			if (i * i != n)
			{
				arr.push_back(n / i);
			}
		}
	}
	return arr;
}

ll n , k ,v[200001] ;

bool can(ll mid)
{
    ll sum = 0 ; 
    for(int i = n/2 ; i<n ; i++)
    {
        if(v[i] < mid)
        sum += mid - v[i];
    }
    return sum<=k;
}
void solve(int test_cases)
{
    cin>> n >> k ;
    for(int i = 0 ; i<n ; i ++)
    {
        cin>>v[i];
    }
    sort(v, v+n);
    ll l = v[n/2] , r = 1e10, ans = v[n/2];

    while(l<=r)
    {   
       ll mid = (l+r) /2;
        if (can(mid))
        {   
            ans = mid;
           l = mid+1;
        }
        else
        {
            r = mid-1;
        }
    }
    cout<<ans<<endl;

}

int main()
{
	
    // freopen("word.in", "r", stdin);
	// freopen("word.out", "w", stdout);
    Abdelraheem;
    int t= 1;

    //cin >> t;
    for (int i = 1; i <= t; i++)
        solve(i);
}

