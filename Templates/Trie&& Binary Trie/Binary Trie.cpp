/*
============================= Binary Trie =============================

BinaryTrie trie;

Operations
----------
trie.insert(x);              // Insert number x.

trie.erase(x);               // Remove one occurrence of x.
                             // Returns true if x existed.

trie.search(x);              // Returns true if x exists.

trie.count(x);               // Number of occurrences of x.

trie.maxXor(x);              // Maximum XOR value with x.

trie.minXor(x);              // Minimum XOR value with x.

trie.maxXorElement(x);       // Element giving maximum XOR with x.

trie.minXorElement(x);       // Element giving minimum XOR with x.

trie.clear();                // Clears the trie.

Complexities
------------
Insert          O(LOG)
Erase           O(LOG)
Search          O(LOG)
Count           O(LOG)
Max XOR         O(LOG)
Min XOR         O(LOG)
Clear           O(1)

Notes
-----
- Supports duplicate numbers.
- erase() removes ONE occurrence.
- LOG = 31 for int, 63 for long long.
- Currently supports numbers in [0, 2^31).

=======================================================================
*/

struct BinaryTrie {

    static const int LOG = 31;
    static const int MAXN = 3200005;

    struct Node {
        int nxt[2];
        int pref, end;

        Node() {
            nxt[0] = nxt[1] = -1;
            pref = end = 0;
        }
    };

    Node tr[MAXN];
    int nodes;

    BinaryTrie() {
        nodes = 1;
    }

    void clear() {
        nodes = 1;
        tr[0] = Node();
    }

    void insert(int x) {
        int cur = 0;
        tr[cur].pref++;

        for (int b = LOG - 1; b >= 0; b--) {
            int bit = (x >> b) & 1;

            if (tr[cur].nxt[bit] == -1) {
                tr[cur].nxt[bit] = nodes;
                tr[nodes] = Node();
                nodes++;
            }

            cur = tr[cur].nxt[bit];
            tr[cur].pref++;
        }

        tr[cur].end++;
    }

    bool search(int x) {
        int cur = 0;

        for (int b = LOG - 1; b >= 0; b--) {
            int bit = (x >> b) & 1;

            if (tr[cur].nxt[bit] == -1)
                return false;

            cur = tr[cur].nxt[bit];
        }

        return tr[cur].end;
    }

    int count(int x) {
        int cur = 0;

        for (int b = LOG - 1; b >= 0; b--) {
            int bit = (x >> b) & 1;

            if (tr[cur].nxt[bit] == -1)
                return 0;

            cur = tr[cur].nxt[bit];
        }

        return tr[cur].end;
    }

    bool erase(int x) {
        if (!search(x))
            return false;

        int cur = 0;
        tr[cur].pref--;

        for (int b = LOG - 1; b >= 0; b--) {
            int bit = (x >> b) & 1;
            cur = tr[cur].nxt[bit];
            tr[cur].pref--;
        }

        tr[cur].end--;
        return true;
    }

    int maxXor(int x) {
        int cur = 0;
        int ans = 0;

        for (int b = LOG - 1; b >= 0; b--) {
            int bit = (x >> b) & 1;
            int want = bit ^ 1;

            if (tr[cur].nxt[want] != -1 &&
                tr[tr[cur].nxt[want]].pref > 0) {

                ans |= (1LL << b);
                cur = tr[cur].nxt[want];
            }
            else
                cur = tr[cur].nxt[bit];
        }

        return ans;
    }

    int minXor(int x) {
        int cur = 0;
        int ans = 0;

        for (int b = LOG - 1; b >= 0; b--) {
            int bit = (x >> b) & 1;

            if (tr[cur].nxt[bit] != -1 &&
                tr[tr[cur].nxt[bit]].pref > 0) {

                cur = tr[cur].nxt[bit];
            }
            else {
                ans |= (1LL << b);
                cur = tr[cur].nxt[bit ^ 1];
            }
        }

        return ans;
    }

    int maxXorElement(int x) {
        int cur = 0;
        int ans = 0;

        for (int b = LOG - 1; b >= 0; b--) {
            int bit = (x >> b) & 1;
            int want = bit ^ 1;

            if (tr[cur].nxt[want] != -1 &&
                tr[tr[cur].nxt[want]].pref > 0) {

                ans |= (1LL * want << b);
                cur = tr[cur].nxt[want];
            }
            else {
                ans |= (1LL * bit << b);
                cur = tr[cur].nxt[bit];
            }
        }

        return ans;
    }

    int minXorElement(int x) {
        int cur = 0;
        int ans = 0;

        for (int b = LOG - 1; b >= 0; b--) {
            int bit = (x >> b) & 1;

            if (tr[cur].nxt[bit] != -1 &&
                tr[tr[cur].nxt[bit]].pref > 0) {

                ans |= (1LL * bit << b);
                cur = tr[cur].nxt[bit];
            }
            else {
                ans |= (1LL * (bit ^ 1) << b);
                cur = tr[cur].nxt[bit ^ 1];
            }
        }

        return ans;
    }
};

/*
============================= Example =============================

int main() {

    BinaryTrie trie;

    trie.insert(5);
    trie.insert(10);
    trie.insert(7);
    trie.insert(5);

    cout << trie.search(5) << '\n';          // 1
    cout << trie.search(8) << '\n';          // 0

    cout << trie.count(5) << '\n';           // 2

    cout << trie.maxXor(6) << '\n';          // Maximum XOR value
    cout << trie.minXor(6) << '\n';          // Minimum XOR value

    cout << trie.maxXorElement(6) << '\n';   // Element giving max XOR
    cout << trie.minXorElement(6) << '\n';   // Element giving min XOR

    trie.erase(5);

    cout << trie.count(5) << '\n';           // 1

    trie.clear();

    return 0;
}

===================================================================
*/
