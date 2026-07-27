#include <bits/stdc++.h>
using namespace std;

void solve() {
    int c, q;
    cin>>c>>q;
    list<int> lru;

    unordered_map<int,list<int>::iterator>mp;
    unordered_map<int,int>dirty;

    int cnt = 0;
    int lstclean = 0;
    while (q--) {
        int op;
        cin >> op;

        if (op == 1 || op == 2) {

            int x;
            cin >> x;

            if (mp.count(x)) {

                lru.erase(mp[x]);
                lru.push_back(x);
                mp[x] = prev(lru.end());

            } else {

                if ((int)lru.size() == c) {

                    int rem = lru.front();
                    lru.pop_front();

                    if (dirty[rem] > lstclean)
                        cnt--;

                    mp.erase(rem);
                    dirty.erase(rem);
                }

                lru.push_back(x);
                mp[x] = prev(lru.end());
                dirty[x] = lstclean;
            }

            if (op == 2 && dirty[x] <= lstclean) {
                dirty[x] = lstclean + 1;
                cnt++;
            }

        } else if (op == 3) {

            cout << cnt << endl;
            cnt = 0;
            lstclean++;

        } else {

            int x;
            cin >> x;

            if (!mp.count(x)) {
                cout << "0 0\n";
            } else {
                cout << "1 " << (dirty[x] > lstclean) << endl;
            }
        }
    }
}



signed main() {
    Abdalraheem();
    int tt = 1;

    cin >> tt;
    while (tt--) {
        solve();
    }
}
