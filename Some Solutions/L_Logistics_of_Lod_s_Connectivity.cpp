#include <bits/stdc++.h>
using namespace std;

int main(){
    ios::sync_with_stdio(false);
    cin.tie(0);
    
    long long N;
    cin >> N;
    
    double net_x = 0.0, net_y = 0.0;
    const double PI = acos(-1.0);
    
    for(long long i = 0; i < N; i++){
        long long a, b;
        cin >> a >> b;
        
        double radians = a * PI / 180.0;
        net_x += b * cos(radians);
        net_y += b * sin(radians);
    }
    
    double distance = sqrt(net_x * net_x + net_y * net_y);
    cout << fixed << setprecision(6) << distance << endl;
}