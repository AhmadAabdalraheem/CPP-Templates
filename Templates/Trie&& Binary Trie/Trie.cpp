#include <bits/stdc++.h>
using namespace std;

/*
================================== Trie ==================================
TIME COMPLEXITIES:
- Insert / Erase / Search / Count : O(|S|)
- Distinct Words                  : O(1)
- Kth Lexicographical String      : O(ALPHA * |S|)
- Clear                           : O(1)
==========================================================================*/

/*
================================== Trie ==================================

Trie trie;

Operations
----------
trie.insert(s);              // Insert string s.

trie.erase(s);               // Remove one occurrence of s.
                              // Returns true if s existed.

trie.search(s);              // Returns true if s exists.

trie.startsWith(pref);       // Returns true if any string has prefix pref.

trie.countWord(s);           // Number of occurrences of s.

trie.countPrefix(pref);      // Number of strings starting with pref.

trie.distinct();             // Number of distinct strings.

trie.kth(k);                 // 1-indexed kth lexicographical string.
                              // Returns "" if k is invalid.

trie.allStrings();           // Returns all strings in lexicographical order.

trie.clear();                // Clears the trie.

Complexities
------------
Insert          O(|S|)
Erase           O(|S|)
Search          O(|S|)
StartsWith      O(|S|)
CountWord       O(|S|)
CountPrefix     O(|S|)
Distinct        O(nodes)
Kth             O(ALPHA * |S|)
AllStrings      O(total characters)
Clear           O(1)

Notes
-----
- Supports duplicate strings.
- erase() removes ONE occurrence.
- Alphabet = 'a' ... 'z'.
- Increase MAXN if needed.

==========================================================================*/

struct Trie {
    static const int ALPHA = 26;
    static const int MAXN = 2000005; // Adjust based on total length of strings

    struct Node {
        int nxt[ALPHA];
        int pref, end;

        void reset() {
            memset(nxt, -1, sizeof(nxt));
            pref = end = 0;
        }
    };

    vector<Node> tr;
    int nodes;
    int distinct_count; // Tracks unique strings in O(1)

    Trie() {
        tr.resize(MAXN);
        clear();
    }

    void clear() {
        nodes = 1;
        distinct_count = 0;
        tr[0].reset();
    }

    void insert(const string &s) {
        int cur = 0;
        tr[cur].pref++;

        for (char c : s) {
            int x = c - 'a';
            if (tr[cur].nxt[x] == -1) {
                tr[cur].nxt[x] = nodes;
                tr[nodes].reset();
                nodes++;
            }
            cur = tr[cur].nxt[x];
            tr[cur].pref++;
        }

        if (tr[cur].end == 0) distinct_count++;
        tr[cur].end++;
    }

    bool search(const string &s) const {
        int cur = 0;
        for (char c : s) {
            int x = c - 'a';
            if (tr[cur].nxt[x] == -1) return false;
            cur = tr[cur].nxt[x];
        }
        return tr[cur].end > 0;
    }

    bool startsWith(const string &s) const {
        int cur = 0;
        for (char c : s) {
            int x = c - 'a';
            if (tr[cur].nxt[x] == -1) return false;
            cur = tr[cur].nxt[x];
        }
        return true;
    }

    int countWord(const string &s) const {
        int cur = 0;
        for (char c : s) {
            int x = c - 'a';
            if (tr[cur].nxt[x] == -1) return 0;
            cur = tr[cur].nxt[x];
        }
        return tr[cur].end;
    }

    int countPrefix(const string &s) const {
        int cur = 0;
        for (char c : s) {
            int x = c - 'a';
            if (tr[cur].nxt[x] == -1) return 0;
            cur = tr[cur].nxt[x];
        }
        return tr[cur].pref;
    }

    bool erase(const string &s) {
        if (!search(s)) return false;

        int cur = 0;
        tr[cur].pref--;

        for (char c : s) {
            int x = c - 'a';
            cur = tr[cur].nxt[x];
            tr[cur].pref--;
        }

        tr[cur].end--;
        if (tr[cur].end == 0) distinct_count--;
        return true;
    }

    // Returns total number of unique strings in O(1)
    int distinct() const {
        return distinct_count;
    }

    // 1-indexed kth lexicographical string
    string kth(int k) const {
        string ans;
        int cur = 0;

        while (true) {
            if (tr[cur].end) {
                if (k <= tr[cur].end) return ans;
                k -= tr[cur].end;
            }

            bool ok = false;
            for (int c = 0; c < ALPHA; c++) {
                int to = tr[cur].nxt[c];
                if (to == -1) continue;

                if (tr[to].pref < k) {
                    k -= tr[to].pref;
                } else {
                    ans += char(c + 'a');
                    cur = to;
                    ok = true;
                    break;
                }
            }
            if (!ok) return "";
        }
    }

    void dfs(int node, string &cur, vector<string> &res) const {
        for (int i = 0; i < tr[node].end; i++)
            res.push_back(cur);

        for (int c = 0; c < ALPHA; c++) {
            int to = tr[node].nxt[c];
            if (to == -1) continue;

            cur += char(c + 'a');
            dfs(to, cur, res);
            cur.pop_back();
        }
    }

    vector<string> allStrings() const {
        vector<string> res;
        string cur;
        dfs(0, cur, res);
        return res;
    }
};

// =========================================================================
// MAIN EXAMPLE
// =========================================================================
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    Trie trie;

    trie.insert("apple");
    trie.insert("app");
    trie.insert("banana");
    trie.insert("apple"); // duplicate

    cout << "Search apple: " << trie.search("apple") << "\n";     // 1
    cout << "Count apple: " << trie.countWord("apple") << "\n";  // 2
    cout << "Distinct words: " << trie.distinct() << "\n";       // 3

    trie.erase("apple");
    cout << "After erasing 1 apple, distinct words: " << trie.distinct() << "\n"; // 3 (apple still exists once)

    trie.erase("apple");
    cout << "After erasing 2nd apple, distinct words: " << trie.distinct() << "\n"; // 2 (apple is gone)

    return 0;
}
