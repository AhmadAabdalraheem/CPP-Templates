#include <bits/stdc++.h>
using namespace std;

void solve() {
    int n, two_s;
    cin >> n >> two_s;
    int s = two_s;
    
    if (n == 3) {
        
        if (two_s % 1 == 0) {
            cout << "YES\n";
            cout << "0 0\n";
            cout << "1 0\n";
            cout << "0 " << s << "\n";
        } else {
            cout << "NO\n";
        }
        return;
    }
    
    if (two_s < n - 2) {
        cout << "NO\n";
        return;
    }
    
    cout << "YES\n";
    
    vector<int> x(n), y(n);
    x[0] = 0; y[0] = 0;
    x[1] = 1; y[1] = 0;
    x[2] = 1; y[2] = 1;
    
    int remaining_area = two_s - 1; 
    
    for (int i = 3; i < n; ++i) {
        if (i % 2 == 1) {
            x[i] = x[i-1] + 1;
            y[i] = y[i-1];
        } else {
            x[i] = x[i-1];
            y[i] = y[i-1] + 1;
        }
        remaining_area -= 1;
    }
    
    if (remaining_area != 0) {
        y[n-1] += remaining_area;
    }
    
    for (int i = 0; i < n; ++i) {
        cout << x[i] << " " << y[i] << "\n";
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int t;
    cin >> t;
    while (t--) {
        solve();
    }
    
    return 0;
}