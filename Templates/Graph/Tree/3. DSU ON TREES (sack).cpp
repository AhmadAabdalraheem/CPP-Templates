#include <bits/stdc++.h>
using namespace std;

const int N = 200005;

// ============================================================================
// 1. TREE STRUCTURE & BASE ARRAYS (FIXED in most problems)
// ============================================================================
vector<int> adj[N];
int sz[N];   // Subtree size for each node
int big[N];  // Heavy Child (child with the maximum subtree size)
int ans[N];  // Stores final answers for each node

// ============================================================================
// 2. GLOBAL STATE / BLACKBOARD [CHANGE per problem]
// ============================================================================
// Add global variables and data structures needed for the state here
int col[N];          // Example: Color of each node
int cnt[N];          // Example: Frequency array (The Blackboard)
int distinct_cnt = 0;// State variable: count of distinct colors

// Function to add a single node u to the global state
inline void add_node(int u) {
    // [CHANGE]: Logic to add node u to state
    if (cnt[col[u]] == 0) distinct_cnt++;
    cnt[col[u]]++;
}

// Function to remove a single node u from the global state
inline void remove_node(int u) {
    // [CHANGE]: Logic to remove node u from state
    cnt[col[u]]--;
    if (cnt[col[u]] == 0) distinct_cnt--;
}

// Function to record the answer for node u from current state
inline void answer_query(int u) {
    // [CHANGE]: Record answer for node u
    ans[u] = distinct_cnt;
}
// ============================================================================


// ============================================================================
// 3. FIXED SACK ENGINE
// ============================================================================

// ----------------------------------------------------------------------------
// Step 1: Precompute subtree sizes and identify heavy children
// ----------------------------------------------------------------------------
void dfs_sz(int u, int p = 0) {
    sz[u] = 1;
    big[u] = -1;
    int max_sz = -1;

    for (int v : adj[u]) {
        if (v == p) continue;
        dfs_sz(v, u);
        sz[u] += sz[v];
        
        // Find child with maximum subtree size
        if (sz[v] > max_sz) {
            max_sz = sz[v];
            big[u] = v; // Mark as Heavy Child
        }
    }
}

// ----------------------------------------------------------------------------
// Step 2: Helper DFS to add or remove an entire subtree
// ----------------------------------------------------------------------------
void add_subtree(int u, int p, int val, int heavy_to_skip) {
    // 1. Add or remove current node u
    if (val == 1) add_node(u);
    else          remove_node(u);

    // 2. Recurse for children (skipping the heavy child if instructed)
    for (int v : adj[u]) {
        if (v != p && v != heavy_to_skip) {
            add_subtree(v, u, val, heavy_to_skip);
        }
    }
}

// ----------------------------------------------------------------------------
// Step 3: Main Sack Algorithm
// ----------------------------------------------------------------------------
void dfs_sack(int u, int p = 0, bool keep = false) {
    
    // 1. Process Light Children and clear their state (keep = false)
    for (int v : adj[u]) {
        if (v != p && v != big[u]) {
            dfs_sack(v, u, false);
        }
    }

    // 2. Process Heavy Child and KEEP its state (keep = true)
    if (big[u] != -1) {
        dfs_sack(big[u], u, true);
    }

    // 3. Add Light Children and current node u to state
    // (Note: big[u] is skipped because its data is already present in state)
    add_subtree(u, p, 1, big[u]);

    // 4. Record answer for node u (state now contains full subtree of u)
    answer_query(u);

    // 5. If keep == false, remove current subtree from state for parent's call
    if (!keep) {
        add_subtree(u, p, -1, -1);
        
        // [Note for Non-invertible States (e.g., Max/Min values)]:
        // Reset max/min variables manually here after clearing the subtree.
    }
}
// ============================================================================

// ============================================================================
// 4. MAIN & DRIVER CODE
// ============================================================================
void solve() {
    int n;
    if (!(cin >> n)) return;

    // [CHANGE]: Read problem inputs (colors, values, etc.)
    for (int i = 1; i <= n; i++) {
        cin >> col[i];
    }

    // Read tree edges
    for (int i = 0; i < n - 1; i++) {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    // 1. Precompute sizes and heavy children
    dfs_sz(1);

    // 2. Run Sack algorithm
    dfs_sack(1);

    // [CHANGE]: Print answers
    for (int i = 1; i <= n; i++) {
        cout << ans[i] << (i == n ? "" : " ");
    }
    cout << "\n";
}

int main() {
    // Fast I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    solve();

    return 0;
}
