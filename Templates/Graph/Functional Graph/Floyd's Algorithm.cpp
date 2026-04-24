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

    // step 3: find cycle length
    int cycle_length = 1;
    int cur = to[cycle_start];
    
    vector<int>path; // the nodes that make the cycle
    
    while (cur != cycle_start) {
        path.push_back(cur);
        cur = to[cur];
        cycle_length++;
    }
    for (auto it : path) {
        ans[it] = cycle_length;
        visited[it] =true;
    }
    return{ cycle_start, cycle_length};
}

