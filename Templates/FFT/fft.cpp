#include <bits/stdc++.h>
using namespace std;

using ll = long long;
using ld = long double;

// Use standard double if long double causes TLE, but ld prevents precision issues
const ld PI = acos(-1.0L);

// Represents a complex number: standard std::complex can be slow on some platforms,
// but a custom struct or the standard one usually suffices. Here is a fast standard usage.
using cd = complex<double>;

// --- Fast Fourier Transform ---
// invert = true performs Inverse FFT (IFFT)
void fft(vector<cd> & a, bool invert) {
    int n = a.size();

    // Bit-reversal permutation (Iterative in-place approach for performance)
    for (int i = 1, j = 0; i < n; i++) {
        int bit = n >> 1;
        for (; j & bit; bit >>= 1)
            j ^= bit;
        j ^= bit;

        if (i < j)
            swap(a[i], a[j]);
    }

    // Cooley-Tukey Radix-2 Butterfly

    for (int len = 2; len <= n; len <<= 1) {
        double ang = 2 * PI / len * (invert ? -1 : 1);

        cd wlen(cos(ang), sin(ang));

        for (int i = 0; i < n; i += len) {
            cd w(1.0, 0.0);

            for (int j = 0; j < len / 2; j++) {
                cd u = a[i+j], v = a[i+j+len/2] * w;

                a[i+j] = u + v;
                a[i+j+len/2] = u - v;
                w *= wlen;
            }
        }
    }

    if (invert) {
        for (cd & x : a)
            x /= n;
    }
}
// --- Polynomial Multiplication ---
// Multiplies two vectors A and B, returning the coefficients of the resulting polynomial
vector<int> multiply(vector<int> const& a, vector<int> const& b) {
    vector<cd> fa(a.begin(), a.end()), fb(b.begin(), b.end());

    // Find the smallest power of 2 that can hold the result size
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

    vector<int> result(n);
    for (int i = 0; i < n; i++)
        result[i] = round(fa[i].real());

    // Trim trailing zeros to keep the size accurate
    //result.resize(a.size() + b.size() - 1);

    return result;
}

int main() {
    // Quick Verification
    // A(x) = 1 + 2x + 3x^2
    // B(x) = 3 + 4x
    vector<ll> A = {1, 2, 3};
    vector<ll> B = {3, 4};

    vector<ll> C = multiply(A, B);

    // Expected Output: 3 10 17 12  -> (3 + 10x + 17x^2 + 12x^3)
    for (ll x : C) {
        cout << x << " ";
    }
    cout << "\n";

    return 0;
}
