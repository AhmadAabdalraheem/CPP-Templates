#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

/*
================================================================================
  PATTERN 4: Min-Plus Matrix Exponentiation (Shortest Path with Exactly K Edges)
================================================================================
  Problem Types & Trigger Criteria:
  - All-pairs shortest paths using EXACTLY K edges.
  - Constraints: N <= 100, K <= 10^9.
  - Time Complexity: O(N^3 * log K).
  
  Core Idea & Modifications:
  - Replace standard matrix multiplication (+, *) with min-plus matrix multiplication (min, +):
    C[i][j] = min_{k} (A[i][k] + B[k][j]).
  - Binary exponentiation of adjacency matrix using Min-Plus multiplication.
================================================================================
*/

namespace MinPlusMatrix {
    // --- DYNAMIC PART ---
    const long long INF = 1e18;

    struct Matrix {
        int n;
        vector<vector<long long>> mat;

        Matrix(int size) : n(size), mat(size, vector<long long>(size, INF)) {}

        static Matrix multiply(const Matrix& A, const Matrix& B) {
            int n = A.n;
            Matrix C(n);
            for (int i = 0; i < n; ++i) {
                for (int k = 0; k < n; ++k) {
                    if (A.mat[i][k] == INF) continue;
                    for (int j = 0; j < n; ++j) {
                        if (B.mat[k][j] == INF) continue;
                        C.mat[i][j] = min(C.mat[i][j], A.mat[i][k] + B.mat[k][j]);
                    }
                }
            }
            return C;
        }

        static Matrix power(Matrix A, long long p) {
            int n = A.n;
            Matrix res(n);
            for (int i = 0; i < n; ++i) res.mat[i][i] = 0; // Identity matrix under Min-Plus

            while (p > 0) {
                if (p & 1) res = multiply(res, A);
                A = multiply(A, A);
                p >>= 1;
            }
            return res;
        }
    };
}

int main_minplus_demo() {
    int n = 3;
    MinPlusMatrix::Matrix adj(n);
    adj.mat[0][1] = 2;
    adj.mat[1][2] = 3;
    adj.mat[0][2] = 10;

    long long K = 2;
    MinPlusMatrix::Matrix res = MinPlusMatrix::Matrix::power(adj, K);

    cout << "Min cost from 0 to 2 with exactly " << K << " edges: " << res.mat[0][2] << "\n";
    return 0;
}
