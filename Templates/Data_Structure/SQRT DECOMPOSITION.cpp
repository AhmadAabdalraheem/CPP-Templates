#include <bits/stdc++.h>
using namespace std;

int n , q ;
vector<int>v;
int SQ = sqrt(n)+1;
vector<vector<int>>b(SQ); //-->blocks
vector<int>block(SQ);    //-->block ans;


void init() {
    for (int i =0 ; i<n ;i++) {
        b[i/SQ].push_back(v[i]);
        block[i/SQ]+=v[i];
    }
}

void update(int idx , int val) {

    int block_idx = idx/SQ ;
    int idx_in_block = idx% SQ;

    block[block_idx] -=v[idx];
    block[block_idx]+=val;
    v[idx] = val;

    b[block_idx][idx_in_block]= val;
}
int query(int l , int r) {
    int ans = 0 ;
    while (l<=r) {

        if ( l%SQ == 0 && l+SQ-1 <= r) {
            ans += block[l/SQ];
            l+=SQ;
        }
        else {
            ans += v[l];
            l++;
        }
    }
    return ans;
}
void solve(){
    cin>>n>>q;
    v.resize(n);
    for (int i =0 ; i<n ;i ++) {
        cin>>v[i];
    }
    init();
    while (q--) {
        int type;
        cin>>type;
        if (type == 1) {
            int idx , val;
            cin>>idx>>val;
            idx--;
            update(idx,val);
        }
        else {
            int l , r;
            cin>>l>>r;
            l--;
            r--;
            cout<<query(l,r)<<endl;
        }
    }
}


signed main() {
    Abdalraheem();
    //freopen("sieve.in", "r", stdin);
    // freopen("where.out", "w", stdout);
    int tt = 1;
   // cin>>tt;
    while (tt--) {
        solve();
    }
}
