#include <bits/stdc++.h>
#define int ll
int oo = 2e18;

struct pt {
    int x , y , u ,v , idx;

};

void solve() {

    int n ;
    cin>>n;
    if (n <= 2) {
        cout << 0 << endl;
        return;
    }

    vector<pt>a(n);


    int ans1 = -oo , ans2 = -oo;
    for (int i =0 ;i<n ;i++) {
        auto& [ x, y ,u , v , idx] = a[i];
        cin>>x>>y;
        u = x + y;
        v = x - y;
        idx = i;


    }
        // ans1 = max(abs(max_u-min_u) , abs(max_v - min_v));
        // cout<<ans1<<endl;

    set<int> st;
    sort(all(a), [](const pt& p1, const pt& p2) {
        return p1.u < p2.u;
    });
    for (int i = 0; i < min(n, 3LL); i++) {
        st.insert(a[i].idx);
        st.insert(a[n - 1 - i].idx);
    }
    sort(all(a), [](const pt& p1, const pt& p2) {
        return p1.v < p2.v;
    });
    for (int i = 0; i < min(n, 3LL); i++) {
        st.insert(a[i].idx);
        st.insert(a[n - 1 - i].idx);
    }
    sort(all(a), [](const pt& p1, const pt& p2) {
        return p1.idx < p2.idx;
    });


    vector<int>useful(all(st));

    auto gett =[&](int skip1 ,int skip2)  {
            int max_u = -oo , min_u = oo , max_v = -oo , min_v = oo ;


            for (auto& it : useful) {
                if (it == skip1 || it ==skip2) {
                    continue;
                }

                 max_u = max(max_u,a[it].u);
                 min_u = min(min_u,a[it].u);
                 max_v = max(max_v , a[it].v);
                 min_v = min(min_v,a[it].v);
            }

        return max( {abs(max_u-min_u) , abs(max_v - min_v) , 0LL });
    };
    int ans = oo;

    // حالة 1: عدم حذف أي نقطة
    ans = min(ans, gett(-1, -1));

    // حالة 2: حذف نقطة واحدة بس
    for (int i = 0; i < useful.size(); i++) {
        ans = min(ans, gett( useful[i], -1));
    }
    // حالة 3: حذف نقطتين
    for (int i = 0; i < useful.size(); i++) {
        for (int j = i + 1; j < useful.size(); j++) {
            ans = min(ans, gett(useful[i], useful[j]));
        }
    }
    cout << ans << endl;

}

signed main() {
    Abdalraheem();
    //freopen("input.txt", "r", stdin);
    //freopen("output.txt", "w", stdout);
    int tt = 1;

    cin>>tt;
    while (tt--) {
        solve();
    }
}
