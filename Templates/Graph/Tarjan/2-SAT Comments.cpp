/*
  ============================================================================
  2-SAT (2-Satisfiability) - Tarjan SCC
  ============================================================================

  WHEN TO THINK ABOUT 2-SAT:
  ----------------------------------------------------------------------------
  Think about 2-SAT when:

  1. You have N boolean variables, and each variable has exactly 2 states:
       TRUE / FALSE
     or each object has exactly 2 possible choices:
       A_i / B_i

  2. The constraints involve at most TWO choices/conditions at a time.

  3. Typical problem statements contain:
       - "At least one of A and B must be chosen."
       - "A and B cannot both be chosen."
       - "If A is chosen, B must be chosen."
       - "If A is chosen, B cannot be chosen."
       - "Choose exactly one of A and B."
       - "A and B must have the same/opposite state."

  4. If brute force would be O(2^N), but every constraint involves only
     two boolean literals, 2-SAT is a strong candidate.

  --------------------------------------------------------------------------
  CORE IDEA:
  ----------------------------------------------------------------------------
  Every variable x_i has two nodes:

       2*i     -> x_i = TRUE
       2*i + 1 -> x_i = FALSE

  A clause:

       (A OR B)

  is converted into two implications:

       (!A -> B)
       (!B -> A)

  Then build the implication graph and find SCCs.

  The formula is SATIFIABLE iff for every variable x_i:

       SCC(x_i) != SCC(!x_i)

  If:

       SCC(x_i) == SCC(!x_i)

  then x_i is forced to be both TRUE and FALSE -> impossible.

  --------------------------------------------------------------------------
  COMMON CONVERSIONS:
  ----------------------------------------------------------------------------

  1. At Least One:
       A OR B

       -> add_clause_or(A, B)

  2. A and B cannot both be chosen:
       !(A AND B)
       = (!A OR !B)

       -> add_clause_or(!A, !B)

  3. If A -> B:
       (!A OR B)

       -> add_clause_or(!A, B)

  4. If A -> !B:
       (!A OR !B)

       -> add_clause_or(!A, !B)

  5. Exactly One of A and B:
       (A OR B) AND (!A OR !B)

       -> add_clause_or(A, B)
       -> add_clause_or(!A, !B)

  6. A XOR B:
       A and B must have opposite values.

       -> add_clause_xor(A, B)

  7. A EQUALS B:
       A and B must have the same value.

       -> add_clause_eq(A, B)

  8. Force A = TRUE:
       -> set_val(A, true)

     Force A = FALSE:
       -> set_val(A, false)

  --------------------------------------------------------------------------
  IMPORTANT RECOGNITION TRICK:
  ----------------------------------------------------------------------------
  If the problem says:

       "For every object, choose one of two options"

  and then gives constraints between pairs of options, try to model each
  option as a boolean literal and build a 2-SAT implication graph.

  Example:

       Person i:
          Option A
          Option B

       "A_i and B_j cannot both happen"

       -> (!A_i OR !B_j)

       -> A_i  -> !B_j
          B_j  -> !A_i

  --------------------------------------------------------------------------
  COMPLEXITY:
  ----------------------------------------------------------------------------
  V = 2*N nodes
  E = O(number of constraints)

  Tarjan SCC:
       Time  : O(V + E)
       Space : O(V + E)

  --------------------------------------------------------------------------
  IMPORTANT:
  ----------------------------------------------------------------------------
  2-SAT works when each constraint can be represented as a clause containing
  at most TWO literals.

  General SAT with 3+ literals is NOT directly solvable using this template.

  --------------------------------------------------------------------------
  USAGE:
  ----------------------------------------------------------------------------

  int n = 4;
  TwoSat ts(n);

  // (x0 OR x1)
  ts.add_clause_or(0, true, 1, true);

  // (!x1 OR x2)
  ts.add_clause_or(1, false, 2, true);

  // x2 XOR x3
  ts.add_clause_xor(2, true, 3, true);

  // x0 = TRUE
  ts.set_val(0, true);

  if (ts.solve()) {
      // SATISFIABLE
      // ts.ans[i] contains the chosen value of x_i
  } else {
      // UNSATISFIABLE
  }

  --------------------------------------------------------------------------
  QUICK MEMORY:
  ----------------------------------------------------------------------------

       2 choices + pairwise constraints
                     ↓
                  2-SAT
                     ↓
             Implication Graph
                     ↓
                   SCC
                     ↓
       x and !x in same SCC ? -> NO
                     ↓
                   YES
  ============================================================================
*/
