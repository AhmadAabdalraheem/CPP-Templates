#include <bits/stdc++.h>
using namespace std;

// cycle detection floyd's algorithm
const int N = 200005;

int to[N];
int n;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> n;
    for (int i = 1; i <= n; i++) {
        cin >> to[i];
    }

    int start = 1; // ممكن تغيرها لأي node

    int slow = start;
    int fast = start;

    // step 1: detect cycle
    do {
        slow = to[slow];
        fast = to[to[fast]];
    } while (slow != fast);

    // step 2: find cycle entry
    slow = start;
    while (slow != fast) {
        slow = to[slow];
        fast = to[fast];
    }

    int cycle_start = slow;

    // step 3: find cycle length
    int cycle_length = 1;
    int cur = to[cycle_start];
    while (cur != cycle_start) {
        cur = to[cur];
        cycle_length++;
    }

    cout << cycle_start << " " << cycle_length << '\n';

    return 0;
}
