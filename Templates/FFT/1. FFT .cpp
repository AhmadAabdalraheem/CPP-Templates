#include <bits/stdc++.h>
using namespace std;

using ll = long long;
using ld = long double;
using cd = complex<double>;

const double PI = acos(-1.0);

// =============================================================================
// TEMPLATE: Fast Iterative Fast Fourier Transform (FFT)
// =============================================================================
// TIME COMPLEXITY  : O(N log N) per multiplication
// SPACE COMPLEXITY : O(N)
// USE CASE         : Real / Floating-point polynomial multiplication (No Modulo)
// =============================================================================

// Iterative In-Place Cooley-Tukey FFT
void fft(vector<cd>& a, bool invert) {
    int n = a.size();

    // Bit-reversal permutation
    for (int i = 1, j = 0; i < n; i++) {
        int bit = n >> 1;
        for (; j & bit; bit >>= 1)
            j ^= bit;
        j ^= bit;

        if (i < j)
            swap(a[i], a[j]);
    }

    // Butterfly operations
    for (int len = 2; len <= n; len <<= 1) {
        double ang = 2 * PI / len * (invert ? -1 : 1);
        cd wlen(cos(ang), sin(ang));

        for (int i = 0; i < n; i += len) {
            cd w(1.0, 0.0);
            for (int j = 0; j < len / 2; j++) {
                cd u = a[i + j];
                cd v = a[i + j + len / 2] * w;
                a[i + j] = u + v;
                a[i + j + len / 2] = u - v;
                w *= wlen;
            }
        }
    }

    if (invert) {
        for (cd& x : a)
            x /= n;
    }
}

// Polynomial Multiplication using FFT: Returns C = A * B
vector<long long> multiply(const vector<long long>& a, const vector<long long>& b) {
    vector<cd> fa(a.begin(), a.end()), fb(b.begin(), b.end());

    int n = 1;
    while (n < (int)a.size() + (int)b.size())
        n <<= 1;

    fa.resize(n);
    fb.resize(n);

    fft(fa, false);
    fft(fb, false);

    for (int i = 0; i < n; i++)
        fa[i] *= fb[i];

    fft(fa, true);

    vector<long long> result(n);
    for (int i = 0; i < n; i++) {
        result[i] = round(fa[i].real());
    }

    // Trim trailing zeros to keep polynomial degree accurate
    while (result.size() > 1 && result.back() == 0) {
        result.pop_back();
    }

    return result;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    // A(x) = 1 + 2x + 3x^2
    // B(x) = 3 + 4x
    vector<long long> A = {1, 2, 3};
    vector<long long> B = {3, 4};

    vector<long long> C = multiply(A, B);

    // Expected Output: 3 10 17 12  -> (3 + 10x + 17x^2 + 12x^3)
    for (long long x : C) {
        cout << x << " ";
    }
    cout << "\n";

    return 0;
}
