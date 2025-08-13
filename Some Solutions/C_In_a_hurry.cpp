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

void solve(int test_cases)
{
	int k , n ;
    cin>>k>>n;
    vi v(n);
    ll ans = INT_MAX;
    ll anss ;
    for(int i = 0 ;i<n ; i++)
    {
        cin>>v[i];
        ll dif = abs( v[i] - k);

        if(dif < ans)
        {
            ans = dif;
            anss  = v[i];
        }
    }
    cout<<ans<< ' '<<  anss <<endl;

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

