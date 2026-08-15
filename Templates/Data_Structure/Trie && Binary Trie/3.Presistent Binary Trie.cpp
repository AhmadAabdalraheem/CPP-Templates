#include <bits/stdc++.h>
using namespace std;

/*
========================= Persistent Binary Trie =========================

PersistentBinaryTrie ptrie;

Operations
----------
ptrie.init();                               // Initializes or resets the root versions.
int new_root = ptrie.insert(prev_root, x);  // Inserts x based on previous version, returns new root.
int query_max = ptrie.maxXor(L_root, R_root, x); // Max XOR of x with elements in version range [L-1, R].
int query_min = ptrie.minXor(L_root, R_root, x); // Min XOR of x with elements in version range [L-1, R].

=========================================================================
WHEN TO USE:
1. Range XOR Queries (Subarray Max/Min XOR):
   - When the problem asks for maximum or minimum XOR (or count of elements satisfying a condition)
     within a specific subarray range [L, R] rather than all elements combined.
2. Tree Path XOR Queries:
   - In tree problems when looking for the maximum or minimum XOR along a path between two nodes (Path XOR),
     where the trie is built based on the parent's version during DFS.
3. Historical State Access / Versioning:
   - When you need to keep track of all previous updates and query the state of the trie
     at any specific point in time or after the first K insertions.
=========================================================================

Complexities
------------
Insert          O(LOG) time & space
Max / Min Query O(LOG) time
Space           O(N * LOG) nodes

Notes
-----
- LOG = 30 for numbers up to 2^30.
- Roots array stores the root index for each prefix version (0-indexed to N).
==========================================================================*/

struct PersistentBinaryTrie {
    static const int LOG = 30;
    static const int MAXN = 300005; // Adjust based on N * LOG (N = max elements/updates)

    struct Node {
        int nxt[2];
        int count;

        Node() {
            nxt[0] = nxt[1] = 0;
            count = 0;
        }
    };

    Node tr[MAXN * LOG];
    int nodes = 0;

    int newNode() {
        nodes++;
        tr[nodes] = Node();
        return nodes;
    }

    // Inserts number x into the trie from the previous root version
    int insert(int prev_root, int x) {
        int root = newNode();
        int cur = root;
        int prev = prev_root;

        tr[cur] = tr[prev];
        tr[cur].count++;

        for (int b = LOG - 1; b >= 0; b--) {
            int bit = (x >> b) & 1;
            
            int next_node = newNode();
            tr[next_node] = tr[tr[prev].nxt[bit]]; // Copy previous child branch
            tr[next_node].count++;

            tr[cur].nxt[bit] = next_node;

            cur = tr[cur].nxt[bit];
            prev = tr[prev].nxt[bit];
        }

        return root;
    }

    // Returns maximum XOR of x using elements present in the version range [L_root, R_root]
    // To query range [L, R] (1-indexed), pass roots[L-1] as L_root and roots[R] as R_root.
    int maxXor(int L_root, int R_root, int x) {
        int curL = L_root;
        int curR = R_root;
        int ans = 0;

        for (int b = LOG - 1; b >= 0; b--) {
            int bit = (x >> b) & 1;
            int want = bit ^ 1;

            // Check if 'want' branch has new elements in the range [L, R]
            int left_count = tr[tr[curR].nxt[want]].count - tr[tr[curL].nxt[want]].count;

            if (left_count > 0) {
                ans |= (1 << b);
                curL = tr[curL].nxt[want];
                curR = tr[curR].nxt[want];
            } else {
                curL = tr[curL].nxt[bit];
                curR = tr[curR].nxt[bit];
            }
        }

        return ans;
    }

    // Returns minimum XOR of x using elements present in the version range [L_root, R_root]
    int minXor(int L_root, int R_root, int x) {
        int curL = L_root;
        int curR = R_root;
        int ans = 0;

        for (int b = LOG - 1; b >= 0; b--) {
            int bit = (x >> b) & 1;

            // Check if 'bit' branch has elements in the range [L, R] to achieve 0 at this bit
            int left_count = tr[tr[curR].nxt[bit]].count - tr[tr[curL].nxt[bit]].count;

            if (left_count > 0) {
                curL = tr[curL].nxt[bit];
                curR = tr[curR].nxt[bit];
            } else {
                ans |= (1 << b);
                curL = tr[curL].nxt[bit ^ 1];
                curR = tr[curR].nxt[bit ^ 1];
            }
        }

        return ans;
    }
};

// =========================================================================
// USAGE EXAMPLE (RANGE QUERIES & TREE PATH XOR)
// =========================================================================
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    PersistentBinaryTrie ptrie;

    // ==========================================
    // 1. Subarray Range Queries Example [L, R]
    // ==========================================
    vector<int> roots;
    roots.push_back(0); // Version 0: Empty trie root

    vector<int> arr = {3, 5, 10, 5};
    for (int i = 0; i < arr.size(); i++) {
        int new_root = ptrie.insert(roots.back(), arr[i]);
        roots.push_back(new_root);
    }

    // Query range [1, 3] (elements: arr[0]=3, arr[1]=5, arr[2]=10) with x = 6
    // L = 1 -> roots[0], R = 3 -> roots[3]
    int max_xor_range = ptrie.maxXor(roots[0], roots[3], 6);
    int min_xor_range = ptrie.minXor(roots[0], roots[3], 6);

    cout << "--- Range Query Example ---" << "\n";
    cout << "Max XOR in range [1, 3] with x=6 is: " << max_xor_range << "\n";
    cout << "Min XOR in range [1, 3] with x=6 is: " << min_xor_range << "\n\n";


    // ==========================================
    // 2. Tree Path XOR Example (DFS + Persistent Trie)
    // ==========================================
    int n = 4;
    vector<int> node_val = {0, 3, 5, 10, 7}; // 1-indexed node values
    vector<vector<int>> adj(n + 1);
    
    // Tree structure:
    //      1 (val 3)
    //     / \
    //    2   3 (val 10)
    //        |
    //        4 (val 7)
    adj[1].push_back(2);
    adj[2].push_back(1);
    
    adj[1].push_back(3);
    adj[3].push_back(1);
    
    adj[3].push_back(4);
    adj[4].push_back(3);

    vector<int> tree_roots(n + 1, 0);
    vector<int> pref_xor(n + 1, 0);

    // DFS to build persistent trie versions along tree paths from root (node 1)
    auto dfs = [&](auto self, int u, int p, int current_root) -> void {
        pref_xor[u] = pref_xor[p] ^ node_val[u];
        tree_roots[u] = ptrie.insert(current_root, pref_xor[u]);

        for (int v : adj[u]) {
            if (v != p) {
                self(self, v, u, tree_roots[u]);
            }
        }
    };

    // Run DFS from root = 1, parent = 0, initial root version = 0
    dfs(dfs, 1, 0, 0);

    cout << "--- Tree Path XOR Example ---" << "\n";
    cout << "Tree paths built successfully via DFS." << "\n";
    
    // Example query: Max/Min XOR path calculation using built tree roots
    int tree_path_max = ptrie.maxXor(tree_roots[0], tree_roots[4], pref_xor[2]);
    int tree_path_min = ptrie.minXor(tree_roots[0], tree_roots[4], pref_xor[2]);
    
    cout << "Tree Path Max XOR query result: " << tree_path_max << "\n";
    cout << "Tree Path Min XOR query result: " << tree_path_min << "\n";

    return 0;
}
