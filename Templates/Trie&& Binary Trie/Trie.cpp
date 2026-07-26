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
    static const int MAXN = 2000005;

    struct Node {
        int nxt[ALPHA];
        int pref, end;

        Node() {
            memset(nxt, -1, sizeof nxt);
            pref = end = 0;
        }
    };

    Node tr[MAXN];
    int nodes;

    Trie() {
        nodes = 1;
    }

    void clear() {
        nodes = 1;
        tr[0] = Node();
    }

    void insert(const string &s) {
        int cur = 0;
        tr[cur].pref++;

        for (char c : s) {
            int x = c - 'a';

            if (tr[cur].nxt[x] == -1) {
                tr[cur].nxt[x] = nodes;
                tr[nodes] = Node();
                nodes++;
            }

            cur = tr[cur].nxt[x];
            tr[cur].pref++;
        }

        tr[cur].end++;
    }

    bool search(const string &s) {
        int cur = 0;

        for (char c : s) {
            int x = c - 'a';

            if (tr[cur].nxt[x] == -1)
                return false;

            cur = tr[cur].nxt[x];
        }

        return tr[cur].end;
    }

    bool startsWith(const string &s) {
        int cur = 0;

        for (char c : s) {
            int x = c - 'a';

            if (tr[cur].nxt[x] == -1)
                return false;

            cur = tr[cur].nxt[x];
        }

        return true;
    }

    int countWord(const string &s) {
        int cur = 0;

        for (char c : s) {
            int x = c - 'a';

            if (tr[cur].nxt[x] == -1)
                return 0;

            cur = tr[cur].nxt[x];
        }

        return tr[cur].end;
    }

    int countPrefix(const string &s) {
        int cur = 0;

        for (char c : s) {
            int x = c - 'a';

            if (tr[cur].nxt[x] == -1)
                return 0;

            cur = tr[cur].nxt[x];
        }

        return tr[cur].pref;
    }

    bool erase(const string &s) {
        if (!search(s))
            return false;

        int cur = 0;
        tr[cur].pref--;

        for (char c : s) {
            int x = c - 'a';
            cur = tr[cur].nxt[x];
            tr[cur].pref--;
        }

        tr[cur].end--;
        return true;
    }

    int distinct() {
        int ans = 0;

        for (int i = 0; i < nodes; i++)
            ans += (tr[i].end > 0);

        return ans;
    }

    string kth(int k) {
        string ans;
        int cur = 0;

        while (true) {

            if (tr[cur].end) {
                if (k <= tr[cur].end)
                    return ans;

                k -= tr[cur].end;
            }

            bool ok = false;

            for (int c = 0; c < ALPHA; c++) {
                int to = tr[cur].nxt[c];

                if (to == -1)
                    continue;

                if (tr[to].pref < k)
                    k -= tr[to].pref;
                else {
                    ans += char(c + 'a');
                    cur = to;
                    ok = true;
                    break;
                }
            }

            if (!ok)
                return "";
        }
    }

    void dfs(int node, string &cur, vector<string> &res) {

        for (int i = 0; i < tr[node].end; i++)
            res.push_back(cur);

        for (int c = 0; c < ALPHA; c++) {
            int to = tr[node].nxt[c];

            if (to == -1)
                continue;

            cur += char(c + 'a');
            dfs(to, cur, res);
            cur.pop_back();
        }
    }

    vector<string> allStrings() {
        vector<string> res;
        string cur;
        dfs(0, cur, res);
        return res;
    }
};


int main() {

    Trie trie;

    // Insert
    trie.insert("apple");
    trie.insert("app");
    trie.insert("banana");
    trie.insert("apple");   // duplicate

    // Search
    cout << trie.search("apple") << '\n';      // 1
    cout << trie.search("orange") << '\n';     // 0

    // Prefix
    cout << trie.startsWith("app") << '\n';    // 1
    cout << trie.startsWith("cat") << '\n';    // 0

    // Count occurrences
    cout << trie.countWord("apple") << '\n';   // 2
    cout << trie.countWord("app") << '\n';     // 1

    // Count strings having prefix
    cout << trie.countPrefix("app") << '\n';   // 3
    // apple, apple, app

    // Distinct strings
    cout << trie.distinct() << '\n';           // 3

    // Lexicographical order (1-indexed)
    cout << trie.kth(1) << '\n';               // app
    cout << trie.kth(2) << '\n';               // apple
    cout << trie.kth(3) << '\n';               // apple
    cout << trie.kth(4) << '\n';               // banana

    // Print all strings
    vector<string> words = trie.allStrings();

    for (auto &s : words)
        cout << s << '\n';

    /*
        Output:
        app
        apple
        apple
        banana
    */

    // Erase one occurrence
    trie.erase("apple");

    cout << trie.countWord("apple") << '\n';   // 1
    cout << trie.distinct() << '\n';           // 3

    // Clear the trie
    trie.clear();

    cout << trie.search("apple") << '\n';      // 0

    return 0;
}
