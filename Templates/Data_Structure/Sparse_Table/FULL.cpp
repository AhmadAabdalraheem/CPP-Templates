struct SparseTable{
    vector<ll>LOG;
    vector<vector<ll>>table;
    ll operation(ll v1, ll v2) { //modify it according to the problem
        return max(v1,v2);
    }
    void prepare(){
        LOG=vector<ll>(N);
        LOG[0]=LOG[1]=0;
        for (ll i = 2; i < N; ++i)
            LOG[i] = LOG[i / 2] + 1;
    }
    void build(vector<ll>&v,ll n) {
        table=vector<vector<ll>>(P,vector<ll>(N));
        for (ll i = 0; i < n; ++i)
            table[0][i] = v[i];
        for (ll p = 1; (1 << p) <= n; ++p)
            for (ll i = 0; i + (1 << p) <= n; ++i)
                table[p][i] = operation(table[p - 1][i], table[p - 1][i + (1 << (p -1))]);
    }
    ll query2(ll l, ll r) {
        ll p = LOG[r - l + 1];
        return operation(table[p][l], table[p][r - (1 << p) + 1]);
    }
    ll query1(ll l,ll r){
        ll ans = 0;
        for (ll p = P - 1; p >= 0; --p) {
            if ((1 << p) <= r - l + 1) {
                ans += table[p][l];
                l += (1 << p);
            }
        }
        return ans;
    }
};
