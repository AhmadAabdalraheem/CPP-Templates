#include <bits/stdc++.h>
using namespace std;

using ll = long long;

int mod = 998244353; 
int root = 3; 

int modpow(int b, int e, int m) {
    int ans = 1;
    for (; e; b = (ll)b * b % m, e /= 2)
        if (e & 1) ans = (ll)ans * b % m;
    return ans;
}

void ntt(vector<int> &a) {
    int n = (int)a.size();
    int L = 31 - __builtin_clz(n);
    
    static vector<int> rt(2, 1); 
    if ((int)rt.size() < n) {
        int k = (int)rt.size(), s = 31 - __builtin_clz(k * 2);
        rt.resize(n);
        for (; k < n; k *= 2, s++) { 
            int z[] = {1, modpow(root, mod >> s, mod)};
            for (int i = k; i < 2 * k; ++i) {
                rt[i] = (ll)rt[i / 2] * z[i & 1] % mod;
            }
        }
    }
    
    vector<int> rev(n);
    for (int i = 0; i < n; ++i) {
        rev[i] = (rev[i / 2] | (i & 1) << L) / 2;
    }
    
    for (int i = 0; i < n; ++i) {
        if (i < rev[i]) swap(a[i], a[rev[i]]);
    }
    
    for (int k = 1; k < n; k *= 2) {
        for (int i = 0; i < n; i += 2 * k) {
            for (int j = 0; j < k; ++j) {
                int z = (ll)rt[j + k] * a[i + j + k] % mod;
                int &ai = a[i + j];
                a[i + j + k] = ai - z + (z > ai ? mod : 0);
                ai += (ai + z >= mod ? z - mod : z);
            }
        }
    }
}

vector<int> multiply(const vector<int> &a, const vector<int> &b) {
    if (a.empty() || b.empty()) return {};
    
    int s = (int)a.size() + (int)b.size() - 1;
    int B = 32 - __builtin_clz(s);
    if ((1 << (B - 1)) == s) B--; 
    int n = 1 << B;
    int inv = modpow(n, mod - 2, mod);
    
    vector<int> L(n, 0), R(n, 0), out(n);
    for (size_t i = 0; i < a.size(); ++i) L[i] = a[i];
    for (size_t i = 0; i < b.size(); ++i) R[i] = b[i];
    
    ntt(L);
    ntt(R);
    
    for (int i = 0; i < n; i++) {
        out[-i & (n - 1)] = (ll)L[i] * R[i] % mod * inv % mod;
    }
    ntt(out);
    
    return {out.begin(), out.begin() + s};
}

// Bounded Polynomial Exponentiation used for grouping identical elements
vector<int> poly_pow(vector<int> a, int k, int max_degree) {
    if (max_degree < 0) return {};
    if (k == 0) return {1};
    
    vector<int> res = {1};
    if ((int)a.size() > max_degree + 1) {
        a.resize(max_degree + 1);
    }

    while (k > 0) {
        if (k & 1) res = multiply(res, a);
        if (k > 1) a = multiply(a, a);
        
        if ((int)res.size() > max_degree + 1) res.resize(max_degree + 1);
        if ((int)a.size() > max_degree + 1) a.resize(max_degree + 1);
        k /= 2;
    }
    return res;
}

// Divide and Conquer merger to combine all unique element polynomials
vector<int> merge_polynomials(int l, int r, const vector<vector<int>> &polys) {
    if (l == r) return polys[l];
    int mid = l + (r - l) / 2;
    vector<int> left_res = merge_polynomials(l, mid, polys);
    vector<int> right_res = merge_polynomials(mid + 1, r, polys);
    return multiply(left_res, right_res);
}

// Computes the subset sum frequencies for non-negative values
// Returns a vector where ans[S] is the number of subsets that sum to S modulo 998244353
vector<int> all_subset_sums(const vector<int> &A) {
    if (A.empty()) return {1}; // Only the empty subset with sum 0

    // Count frequencies of each element to minimize redundant exponentiations
    map<int, int> counts;
    long long total_sum = 0;
    for (int x : A) {
        counts[x]++;
        total_sum += x;
    }

    vector<vector<int>> base_polys;
    for (auto const& [val, freq] : counts) {
        if (val == 0) {
            // A zero value can be taken 'freq' times, scaling the total ways by 2^freq
            // Represented by the polynomial: (1 + y^0)^freq = 2^freq at index 0
            int multiplier = modpow(2, freq, mod);
            base_polys.push_back({multiplier});
        } else {
            // For a distinct item, its base form is (1 + 0*y + 0*y^2 ... + 1*y^val)
            vector<int> item_poly(val + 1, 0);
            item_poly[0] = 1;
            item_poly[val] = 1;

            // Raise it to its frequency count bounded by total possible sum limits
            base_polys.push_back(poly_pow(item_poly, freq, total_sum));
        }
    }

    // Combine all generated polynomials using divide and conquer
    return merge_polynomials(0, (int)base_polys.size() - 1, base_polys);
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    // Subsets of {1, 2, 2}:
    // {}->0, {1}->1, {2}->2, {2}->2, {1,2}->3, {1,2}->3, {2,2}->4, {1,2,2}->5
    // Expected counts: 0:1, 1:1, 2:2, 3:2, 4:1, 5:1
    vector<int> A = {1, 2, 2}; 
    
    vector<int> subset_sums = all_subset_sums(A);

    cout << "Subset Sum Frequencies:\n";
    for (size_t i = 0; i < subset_sums.size(); ++i) {
        if (subset_sums[i] > 0) {
            cout << "Sum " << i << ": " << subset_sums[i] << " times\n";
        }
    }

    return 0;
}
