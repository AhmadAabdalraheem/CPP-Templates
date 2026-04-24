#include <bits/stdc++.h>
using namespace std;

// cycle detection floyd's algorithm
const int N = 200005;

int to[N];
int n;
// works from one start node and gets the cycle in this component
pair<int,int>floyd(int node) {
    int start = node; // ممكن تغيرها لأي node

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

    // build cycle path
    vector<int> cycle;
    cycle.push_back(cycle_start);

    int cur = to[cycle_start];
    while (cur != cycle_start) {
        cycle.push_back(cur);
        cur = to[cur];
    }

    int cycle_length = cycle.size();

    return {cycle_start, cycle_length};
}

