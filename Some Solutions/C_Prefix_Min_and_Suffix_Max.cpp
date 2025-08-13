#include <bits/stdc++.h>
#define Abdalraheem  ios_base::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr);
using namespace std;
#define ll long long
#define ld long double
#define ull unsigned long long
#define endl '\n'
#define vi vector<int>
#define vl vector<long long>
#define vii vector<vector<int>>
#define co(x) __builtin_popcountll(x)
#define ctz(x) __builtin_ctzll(x)
#define clz(x) __builtin_clzll(x)
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
#define PI acos(-1)
ll gcd(ll a, ll b)    {if (b == 0) return a; return gcd(b, a % b);} //O(log min(a, b))
ll lcm(ll a,ll b)     {return a/gcd(a,b)*b;}
ll summ(ll n)         {return  n * ( n+1)/2;}

////////////////////////////////////////Sol:////////////////////////////////////////

void solve(int test_cases)
{	
    ll n ;
    cin>>n;
    vi v(n);
    cin(v);
    vi pref(n, false);
    int min = v[0];
    pref[0] = true;
    for (int i = 1; i < n; i++)
    {
        if (v[i] < min)
        {
            min = v[i];
            pref[i] = true;
        }
    }

    vi suf(n, false);
    int max = v[n - 1];
    suf[n - 1] = true;
    for (int i = n - 2; i >= 0; --i)
    {
        if (v[i] > max)
        {
            max = v[i];
            suf[i] = true;
        }
    }
    string res= "";
    for (int i = 0; i < n; ++i)
    {
        if (pref[i] || suf[i])
        {
            res += '1';
        }
        else
        {
            res += '0';
        }
    }

    cout << res << endl;
}

signed main()
{
	
	// freopen("divsnums.in", "r", stdin);
	// freopen("divsnums.out", "w", stdout);
    Abdalraheem;
    int t= 1;
	
    cin >> t;
    for (int i = 1; i <= t; i++)
	solve(i);
}

//----------------------------------------------Functions-----------------------------------------------------//
vector<ll> primefactors(ll n ) //O(SQRT(N))
{
    vector<ll> fact;
    for(ll i = 2;  i*i <= n ;i++)
    {
        while(n% i == 0)
        {
            fact.push_back(i);
            n/=i;
        }
    }
    if(n>1)
    fact.push_back(n);
    return fact; 
}

int const N = 1e6 + 5;
bool prime[N] ;
void seiveofEratosthenes() //O(N * LOG(LOG(N)))
{
    memset(prime, true, sizeof prime );
    prime[0] = prime[1] = 0;
    for(ll i = 2;  i*i <=N; i++) //primes from 1 to n 
    {
        if( prime[i] == 1)
        {
            for(ll j = i+i ; j<=N ; j+=i)
            {
                prime[j]  = 0;
            }
        }
    }
    
}

vl spf;
void Spf(ll n)
{
    spf.resize(n + 1);
    for (int i = 0; i <= n; i++)
        spf[i] = i;
    for (ll i = 2; i * i < N; i++) // smallest prime factor 
    {
        if (spf[i] == i)
        {
            for (ll j = i + i; j <= N; j += i)
            {
                spf[j] = min(spf[j], i);
            }
        }
    }
}
vector<ll > primefactors2 (ll n) // O(LOG(N))
{
    vector<ll>factors;
    while(n > 1)
    {
        factors.push_back(spf[n]);
        n/= spf[n];
    }
    return factors;
}
vector<long long> divisor(long long n) // O(SQRT(N))
{
	vector<long long> divisors;
	for (long long d = 2; d *d <= n; d++)
	{
		if(n % d == 0)
		{
			divisors.push_back(d);
			if (d != n / d) 
			{
				divisors.push_back(n / d);
			}
		}
	}
	return divisors;
}

bool is_prime(ll x) { // O(SQRT(N)))
	if (x < 2) return 0;
	
    for (ll i = 2; i * i <= x; ++i) 
	{
		if (x % i == 0) 
		return 0;
    }
    return 1;
}

string to_binary(ll n)
{
	string s;
	while (n > 0)
	{
		int rem = n % 2;
		
		s.push_back(char(rem + '0'));
		n /= 2;
	}
	reverse(all(s));
	return s;
}
// multiply x by 2  i times
int shift_left(int x,  int i )
{
	return x << i;
}
// divide x by 2  i times
int right_left(int x,  int i )
{
	return x >> i;
}
//set ith bit
int addbit(int x , int i )
{
	return x | (1<<i);
}
//unset ith bit
int removebit(int x , int i )
{
	return x & (~(1<<i));
}
//check the existence of the ith bit
bool checkbit(int x , int i )
{
	return (x>>i) & 1;
}

//flip the bit
int flipbite(int x, int i )
{
	return x ^ (1<<i);
}
//get the first set bit from right    least significant bit
int LSB(int x)
{
	return x & (-x);
}
//get the first set bit from left    most significant bit
int MSB(int x) //O(LOG(N))
{
	int msb = 0;
	while(x)
	{
		 x>>=1; //  x/=2;
		msb++;
	}
	return msb-1;

}
bool ispoweroftwo(int x)
{
	return !( x & (x-1));
}
