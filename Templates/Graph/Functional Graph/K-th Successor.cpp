#include <bits/stdc++.h>
using namespace std;

const int N = 200005;
const int LOG = 30;

int n, q;
int to[N];
int up[N][LOG];

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> n >> q;

    for (int i = 1; i <= n; i++) {
        cin >> to[i];
        up[i][0] = to[i];
    }

    // build table
    for (int j = 1; j < LOG; j++) {
        for (int i = 1; i <= n; i++) {
            up[i][j] = up[up[i][j-1]][j-1];
        }
    }

    // queries
    while (q--) {
        int x, k;
        cin >> x >> k;

        for (int j = 0; j < LOG; j++) {
            if (k & (1 << j)) {
                x = up[x][j];
            }
        }

        cout << x << '\n';
    }

    return 0;
}
