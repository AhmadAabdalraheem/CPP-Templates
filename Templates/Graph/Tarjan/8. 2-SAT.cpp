#include <bits/stdc++.h>
using namespace std;

/*
  ============================================================================
  2-SAT (2-Satisfiability) Solver using Tarjan's SCC Algorithm
  ============================================================================

  WHEN TO USE:
  1. Boolean Satisfiability problems where constraints are given as clauses of 2 variables.
  2. Logic conditions like:
     - (A OR B)
     - (A XOR B)
     - (A EQUALS B)
     - Forcing variable A to be TRUE or FALSE.
     - "At most one" or "At least one" constraints between pairs.

  HOW IT WORKS:
  - Variable i (0-indexed) maps to 2 graph nodes:
      2 * i     --> True state (x_i)
      2 * i + 1 --> False state (!x_i)
  - Implication Rule: (A OR B) == (!A -> B) AND (!B -> A).
  - Solution exists iff x_i and !x_i do not belong to the same Strongly Connected Component (SCC).
*/

struct TwoSat {
    int n;
    vector<vector<int>> adj;
    vector<bool> ans; // Stores truth assignment for variables 0..n-1

    // Tarjan's SCC variables
    vector<int> dfn, low, comp;
    vector<bool> in_stack;
    stack<int> st;
    int timer, scc_cnt;

    TwoSat(int n) : n(n), adj(2 * n), ans(n) {}

    // Helper to get node index for variable u
    int node(int u, bool status) {
        return 2 * u + (status ? 0 : 1);
    }

    // Add direct implication: u -> v
    void add_implication(int u, bool status_u, int v, bool status_v) {
        adj[node(u, status_u)].push_back(node(v, status_v));
    }

    // 1. Clause: (u OR v) == (status_u OR status_v)
    void add_clause_or(int u, bool status_u, int v, bool status_v) {
        add_implication(u, !status_u, v, status_v);
        add_implication(v, !status_v, u, status_u);
    }

    // 2. Force variable u to be a fixed boolean value
    void set_val(int u, bool status) {
        // (!status -> status) forces status to be TRUE
        add_implication(u, !status, u, status);
    }

    // 3. Clause: (u XOR v) == true (u and v have opposite values)
    void add_clause_xor(int u, bool status_u, int v, bool status_v) {
        add_clause_or(u, status_u, v, status_v);
        add_clause_or(u, !status_u, v, !status_v);
    }

    // 4. Clause: (u EQUALS v) (u and v have identical values)
    void add_clause_eq(int u, bool status_u, int v, bool status_v) {
        add_clause_or(u, status_u, v, !status_v);
        add_clause_or(u, !status_u, v, status_v);
    }

    void dfs(int u) {
        dfn[u] = low[u] = ++timer;
        st.push(u);
        in_stack[u] = true;

        for (int v : adj[u]) {
            if (!dfn[v]) {
                dfs(v);
                low[u] = min(low[u], low[v]);
            } else if (in_stack[v]) {
                low[u] = min(low[u], dfn[v]);
            }
        }

        if (low[u] == dfn[u]) {
            scc_cnt++;
            while (true) {
                int v = st.top();
                st.pop();
                in_stack[v] = false;
                comp[v] = scc_cnt;
                if (u == v) break;
            }
        }
    }

    // Returns true if satisfiable, false otherwise
    bool solve() {
        dfn.assign(2 * n, 0);
        low.assign(2 * n, 0);
        comp.assign(2 * n, 0);
        in_stack.assign(2 * n, false);
        timer = scc_cnt = 0;

        for (int i = 0; i < 2 * n; ++i) {
            if (!dfn[i]) dfs(i);
        }

        for (int i = 0; i < n; ++i) {
            // Contradiction: variable and its negation are in the same SCC
            if (comp[2 * i] == comp[2 * i + 1]) {
                return false;
            }
            // Tarjan assigns component IDs in reverse topological order
            ans[i] = comp[2 * i] < comp[2 * i + 1];
        }

        return true;
    }
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    /*
      Example Problem:
      N = 4 variables (0, 1, 2, 3)

      Constraints:
      1. (x0 OR x1)
      2. (!x1 OR x2)
      3. (x2 XOR x3)
      4. Force x0 = TRUE
    */

    int n = 4;
    TwoSat ts(n);

    // 1. (x0 == true OR x1 == true)
    ts.add_clause_or(0, true, 1, true);

    // 2. (x1 == false OR x2 == true)
    ts.add_clause_or(1, false, 2, true);

    // 3. (x2 XOR x3)
    ts.add_clause_xor(2, true, 3, true);

    // 4. Force x0 = true
    ts.set_val(0, true);

    if (ts.solve()) {
        cout << "SATISFIABLE\n";
        for (int i = 0; i < n; ++i) {
            cout << "x" << i << " = " << (ts.ans[i] ? "TRUE" : "FALSE") << "\n";
        }
    } else {
        cout << "UNSATISFIABLE\n";
    }

    return 0;
}
