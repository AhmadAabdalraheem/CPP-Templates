/* Statemet
You are playing a game consisting of n planets. 
Each planet has a teleporter to another planet (or the planet itself).
You start on a planet and then travel through teleporters until you reach a planet that you have already visited before.
Your task is to calculate for each planet the number of teleportations there would be if you started on that planet.
Input:

5
2 4 3 1 4
Output:

3 3 1 3 4*/

#include <bits/stdc++.h>
using namespace std;
#define ll long long

vector<int>to;
vector<int>ans;
vector<bool>visited;
//Don't Forget DSU

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


    for (auto it: cycle) {
        visited[it]=true;
        ans[it]=cycle_length;
    }
    return {cycle_start, cycle_length};
}

int dfs(int u) {
    if (ans[u]) return ans[u];
    return ans[u] = dfs(to[u]) + 1;
}
void solve() {
    //Did it reach her ??
    int n ;
    cin>>n;
    to.resize(n+3);
    visited.assign(n+3,false);
    ans.resize(n+1);
    DSU d;
    d.init(n+3);
    for (int i =1 ;i<=n;i++) {
        cin>>to[i];
        d.merge(i,to[i]);

    }

    for (int i =1 ; i<=n;i++) {
        if (i == d.find_root(i)) {
            floyd(i);
        }
    }
    for (int i =1 ; i<=n;i++) {
        if (!ans[i]) {
            dfs(i);
        }
    }
    for (int i =1; i<=n;i++) {
        cout<<ans[i]<<' ';
    }

}
