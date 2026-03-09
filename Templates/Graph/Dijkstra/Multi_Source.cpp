#include <bits/stdc++.h>
using namespace std;

/*
Multi-Source Dijkstra

Idea:
- Same idea as Dijkstra but instead of starting from one node,
  we start from multiple sources.
- Push all source nodes into the priority queue with distance = 0.
- Then run normal Dijkstra.

Use When:
- You want the shortest distance from the closest source among many.
- Example: nearest hospital, nearest police station, nearest special node.

Time Complexity:
O((V + E) log V)
*/

const long long INF = 1e18;
const int N = 200005;

vector<pair<int,int>> adj[N];
long long dist[N];

void multisource_dijkstra(int n, vector<int> sources) {

    priority_queue<
        pair<long long,int>,
        vector<pair<long long,int>>,
        greater<pair<long long,int>>
    > pq;

    for(int i = 1; i <= n; i++) dist[i] = INF;

    for(int s : sources) {
        dist[s] = 0;
        pq.push({0, s});
    }

    while(!pq.empty()) {

        auto [d,u] = pq.top();
        pq.pop();

        if(d > dist[u]) continue;

        for(auto [v,w] : adj[u]) {

            if(dist[v] > dist[u] + w) {

                dist[v] = dist[u] + w;
                pq.push({dist[v], v});

            }
        }
    }
}
