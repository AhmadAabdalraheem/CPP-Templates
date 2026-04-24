#include <bits/stdc++.h>
#define int ll

int n,m, k;
int to[N];
void solve() {
    //Did it reach her ??
    cin >> n >> m >> k;

    vector<int> l(n+1), r(n+1);
    for (int i = 1; i <= n; i++) {
        cin >> l[i] >> r[i];
    }

    vector<char> seq(m+1);
    for (int i = 1; i <= m; i++) {
        cin >> seq[i];
    }

    vector<int> next(n+1);
    for (int p = 1; p <= n; p++) {
        int cur = p;
        for (int j = 1; j <= m; j++) {
            if (seq[j] == 'L')
                cur = l[cur];
            else
                cur = r[cur];
        }
        next[p] = cur;
    }

    int LOG = 0;
    while ((1LL << LOG) <= k) {
        LOG++;
    }

    vector<vector<int>> sparse(n+1, vector<int>(LOG));

    for (int i = 1; i <= n; i++) {
        sparse[i][0] = next[i];
    }
    for (int j = 1; j < LOG; j++) {
        for (int i = 1; i <= n; i++) {
            sparse[i][j] = sparse[sparse[i][j-1]][j-1];
        }
    }

    int ans = 1;
    for (int j = 0; j < LOG; j++) {
        if (k & (1 << j)) {
            ans = sparse[ans][j];
        }
    }
    cout<<ans;
}
