#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>

using namespace std;

struct LCA {
    int n;
    int max_log;
    vector<int> depth;
    vector<vector<int>> up;

    void init(int num_nodes) {
        n = num_nodes;
        max_log = ceil(log2(n)) + 1;
        depth.assign(n + 1, 0);
        up.assign(n + 1, vector<int>(max_log, 0));
    }

    void dfs(int u, int p, const vector<vector<int>>& adj) {
        up[u][0] = p;
        for (int i = 1; i < max_log; ++i) {
            if (up[u][i - 1] != -1) {
                up[u][i] = up[up[u][i - 1]][i - 1];
            } else {
                up[u][i] = -1;
            }
        }

        for (int v : adj[u]) {
            if (v != p) {
                depth[v] = depth[u] + 1;
                dfs(v, u, adj);
            }
        }
    }

    void build(int root, const vector<vector<int>>& adj) {
        depth[root] = 0;
        dfs(root, -1, adj);
    }

    int query(int u, int v) {
        if (depth[u] < depth[v]) {
            swap(u, v);
        }

        int diff = depth[u] - depth[v];
        for (int i = 0; i < max_log; ++i) {
            if ((diff >> i) & 1) {
                u = up[u][i];
            }
        }

        if (u == v) return u;

        for (int i = max_log - 1; i >= 0; --i) {
            if (up[u][i] != up[v][i]) {
                u = up[u][i];
                v = up[v][i];
            }
        }

        return up[u][0];
    }

    int get_distance(int u, int v) {
        return depth[u] + depth[v] - 2 * depth[query(u, v)];
    }
};


int nodes = 7;
    
    // Create adjacency list (1-indexed representation)
    vector<vector<int>> adj(nodes + 1);
    
    // Define tree edges
    adj[1].push_back(2); adj[2].push_back(1);
    adj[1].push_back(3); adj[3].push_back(1);
    adj[2].push_back(4); adj[4].push_back(2);
    adj[2].push_back(5); adj[5].push_back(2);
    adj[3].push_back(6); adj[6].push_back(3);
    adj[5].push_back(7); adj[7].push_back(5);

    // Initialize and build the black box
    LCA treeLCA;
    treeLCA.init(nodes);
    treeLCA.build(1, adj); // 1 is the root

    // Query examples
    cout << "LCA of 4 and 7: " << treeLCA.query(4, 7) << endl; // Expected: 2
    cout << "LCA of 4 << and 6: " << treeLCA.query(4, 6) << endl; // Expected: 1
    cout << "LCA of 5 and 7: " << treeLCA.query(5, 7) << endl; // Expected: 5

    // Distance examples
    cout << "Distance between 4 and 7: " << treeLCA.get_distance(4, 7) << endl; // Expected: 3
