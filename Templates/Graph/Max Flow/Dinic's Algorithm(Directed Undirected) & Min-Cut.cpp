#include <bits/stdc++.h>
using namespace std;

/*
 * -----------------------------------------------------------------------------
 * TEMPLATE: Dinic's Algorithm with Hybrid (Directed/Undirected) & Min-Cut
 * -----------------------------------------------------------------------------
 * Time Complexity : O(V^2 * E) for Max Flow / Min Cut.
 * Space Complexity: O(V + E)
 * -----------------------------------------------------------------------------
 */

struct Dinic {
    struct Edge {
        int from;
        int to;
        long long cap;
        int rev;          // Index of the reverse edge in adj[to]
        bool is_original; // True if this edge was explicitly added by the user
    };

    int n;
    vector<vector<Edge>> adj;
    vector<int> level;
    vector<int> ptr;
    vector<bool> visited_in_cut;

    // Constructor to initialize the graph with N vertices
    Dinic(int n) : n(n), adj(n), level(n), ptr(n), visited_in_cut(n) {}

    // Function to clear the graph between multiple testcases
    void clear() {
        fill(level.begin(), level.end(), -1);
        fill(ptr.begin(), ptr.end(), 0);
        for (int i = 0; i < n; ++i) {
            adj[i].clear();
        }
    }

    /*
     * add_edge function:
     * - For Directed Graphs  : Set 'undirected = false' (Default). Reverse capacity is 0.
     * - For Undirected Graphs: Set 'undirected = true'. Reverse capacity equals forward capacity.
     */
    void add_edge(int u, int v, long long cap, bool undirected = false) {
        adj[u].push_back({u, v, cap, (int)adj[v].size(), true});
        adj[v].push_back({v, u, (undirected ? cap : 0), (int)adj[u].size() - 1, undirected});
    }

    // BFS to build the level graph
    bool bfs(int s, int t) {
        fill(level.begin(), level.end(), -1);
        level[s] = 0;
        queue<int> q;
        q.push(s);
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            for (auto &edge : adj[u]) {
                if (edge.cap > 0 && level[edge.to] == -1) {
                    level[edge.to] = level[u] + 1;
                    q.push(edge.to);
                }
            }
        }
        return level[t] != -1; // Returns true if the sink is reachable
    }

    // DFS to find blocking flows in the level graph
   long long dfs(int u, int t, long long pushed) {
    if (pushed == 0 || u == t) return pushed;

    long long pushed_total = 0;

    for (int &cid = ptr[u]; cid < adj[u].size(); ++cid) {
        auto &edge = adj[u][cid];
        int v = edge.to;

        if (level[u] + 1 != level[v] || edge.cap == 0) continue;

        long long tr = dfs(v, t, min(pushed, edge.cap));
        if (tr == 0) continue; // Skip if this path can't reach the sink

        // Update residual capacities
        edge.cap -= tr;
        adj[v][edge.rev].cap += tr;

        pushed_total += tr;
        pushed -= tr;

        // If the remaining capacity coming into 'u' is exhausted, we can stop
        if (pushed == 0) break;
    }

    // Optional optimization: If this node couldn't push anything at all,
    // isolate it so future DFS calls in this phase ignore it entirely.
    if (pushed_total == 0) level[u] = -1;

    return pushed_total;
}

    // Main function to calculate Maximum Flow (which equals Min Cut Value)
    long long get_max_flow(int s, int t) {
        long long flow = 0;
        while (bfs(s, t)) {
            fill(ptr.begin(), ptr.end(), 0);
            while (long long pushed = dfs(s, t, 2e18)) { // 2e18 behaves as Infinity
                flow += pushed;
            }
        }
        return flow;
    }

    // Function to extract the actual edges that form the Minimum Cut
    vector<pair<int, int>> get_min_cut_edges(int s) {
        fill(visited_in_cut.begin(), visited_in_cut.end(), false);
        queue<int> q;
        q.push(s);
        visited_in_cut[s] = true;

        // 1. Traverse reachable nodes from source using remaining residual capacity (cap > 0)
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            for (auto &edge : adj[u]) {
                if (edge.cap > 0 && !visited_in_cut[edge.to]) {
                    visited_in_cut[edge.to] = true;
                    q.push(edge.to);
                }
            }
        }

        // 2. Identify cut edges (Original edges going from a visited node to an unvisited node)
        vector<pair<int, int>> cut_edges;
        for (int u = 0; u < n; u++) {
            if (visited_in_cut[u]) {
                for (auto &edge : adj[u]) {
                    if (edge.is_original && !visited_in_cut[edge.to]) {
                        cut_edges.push_back({u, edge.to});
                    }
                }
            }
        }
        return cut_edges;
    }


    //Function to reconstruct and return all individual paths from S to T
    vector<vector<int>> get_max_flow_paths(int s, int t) {
        vector<FlowEdgeOutput> used = get_used_edges();

        // Build an internal adjacency list containing only used flow channels
        // We use a pair of {to_node, flow_amount}
        vector<vector<pair<int, long long>>> flow_adj(n);
        for (auto &edge : used) {
            flow_adj[edge.from].push_back({edge.to, edge.flow});
        }

        vector<vector<int>> paths;

        // Keep extracting paths as long as there is remaining flow coming out of Source
        while (true) {
            vector<int> current_path;
            int curr = s;
            current_path.push_back(curr);

            bool found_path = false;

            // Reconstruct a single path using DFS-like step traversal
            while (curr != t) {
                bool advanced = false;
                for (auto &edge : flow_adj[curr]) {
                    if (edge.second > 0) { // If this branch still has remaining pushed flow
                        edge.second--;     // Consume 1 unit of flow for this path
                        curr = edge.first;
                        current_path.push_back(curr);
                        advanced = true;
                        break;
                    }
                }
                if (!advanced) break; // Dead end or no more flow out of this node
            }

            // If we successfully traced a path from S to T, save it
            if (current_path.back() == t) {
                paths.push_back(current_path);
            } else {
                break; // No more complete paths can be formed
            }
        }
        return paths;
    }




    // Struct to represent the output of edges that actually carried flow
    struct FlowEdgeOutput {
        int from, to;
        long long flow;
        long long max_cap;
    };

    // Function to extract all original edges that were used in the maximum flow
    vector<FlowEdgeOutput> get_used_edges() {
        vector<FlowEdgeOutput> used_edges;
        for (int u = 0; u < n; u++) {
            for (auto &edge : adj[u]) {
                // We only care about real edges added by the user, not the artificial reverse ones
                if (edge.is_original) {
                    // The current capacity of the reverse edge holds exactly the amount of flow
                    // that was pushed through this forward edge.
                    long long actual_flow = adj[edge.to][edge.rev].cap;

                    // Total original capacity is the remaining capacity + the pushed flow
                    long long original_cap = edge.cap + actual_flow;

                    // If flow was pushed through this edge, record it
                    if (actual_flow > 0) {
                        used_edges.push_back({edge.from, edge.to, actual_flow, original_cap});
                    }
                }
            }
        }
        return used_edges;
    }
};


void solve(){
    int vertices, edges;
  cin >> vertices >> edges;

    // Define source (S) as node 0 and sink (T) as the last node
    int S = 0;
    int T = vertices - 1;

    // 1. Initialize the Dinic solver object with the number of vertices
    Dinic solver(vertices);

    // 2. Read all edges dynamically via user input loop
    for (int i = 0; i < edges; i++) {
        int u, v;
        long long cap;
        cin >> u >> v >> cap;

        // Add edge to the network
        solver.add_edge(u, v, cap); // Default is directed. Use solver.add_edge(u, v, cap, true) if undirected.
    }

    cout << "\n-------------------------------------------\n";

    // 3. Compute and print the Maximum Flow
    long long max_flow = solver.get_max_flow(S, T);
    cout << "Maximum Flow from " << S << " to " << T << " is: " << max_flow << "\n";
    cout << "-------------------------------------------\n";

    vector<vector<int>> routes = flow.get_max_flow_paths(0, n-1);

    // 2. Print total number of days (which is the size of the paths vector)
    //cout << routes.size() << "\n"; -->max flow

    // 3. Print each route
    for (auto &path : routes) {
        cout << path.size() << "\n";
        for (int i = 0; i < path.size(); i++) {
            cout<<path[i] + 1 <<' ';
        }
        cout << "\n";


    
    // 4. Print all original edges that actually carried flow
    cout << "Edges that carried flow:\n";
    vector<Dinic::FlowEdgeOutput> flows = solver.get_used_edges();
    for (auto &edge : flows) {
        cout << "Edge (" << edge.from << " -> " << edge.to << ") | "
             << "Flow: " << edge.flow << " / " << edge.max_cap << "\n";
    }
    cout << "-------------------------------------------\n";

    // 5. Print all original edges forming the Minimum Cut
    cout << "Edges forming the Minimum Cut:\n";
    vector<pair<int, int>> cut_edges = solver.get_min_cut_edges(S);
    for (auto &edge : cut_edges) {
        cout << "Cut Edge: " << edge.first << " -> " << edge.second << "\n";
    }
    cout << "-------------------------------------------\n";
}

int main() {
    // Fast I/O for Competitive Programming
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int t = 1;
    // cin >> t; // Uncomment if the problem has multiple test cases
    while (t--) {
        solve();
    }
    return 0;
}
