#include <bits/stdc++.h>
using namespace std;
 
/*
RANGE:
- لحد ~2000-5000 digit
- كل الأرقام موجبة
*/
 
/*
COMPARE
- هل a < b
- O(n)
*/
bool smaller(string a, string b) {
    if (a.size() != b.size()) return a.size() < b.size();
    return a < b;
}
 
/*
ADD
- a + b
- O(n)
- استخدام: جمع أرقام كبيرة
*/
string add(string a, string b) {
    if (a.size() < b.size()) swap(a, b);
    int carry = 0;
    string res = "";
    int n = a.size(), m = b.size();
 
    for (int i = 0; i < n; i++) {
        int x = a[n - 1 - i] - '0';
        int y = (i < m ? b[m - 1 - i] - '0' : 0);
        int sum = x + y + carry;
        res += char(sum % 10 + '0');
        carry = sum / 10;
    }
 
    if (carry) res += char(carry + '0');
    reverse(res.begin(), res.end());
    return res;
}
 
/*
SUB
- a - b (لازم a >= b)
- O(n)
- استخدام: طرح
*/
string sub(string a, string b) {
    string res = "";
    int borrow = 0;
    int n = a.size(), m = b.size();
 
    for (int i = 0; i < n; i++) {
        int x = a[n - 1 - i] - '0';
        int y = (i < m ? b[m - 1 - i] - '0' : 0);
        x -= borrow;
 
        if (x < y) {
            x += 10;
            borrow = 1;
        } else borrow = 0;
 
        res += char((x - y) + '0');
    }
 
    while (res.size() > 1 && res.back() == '0') res.pop_back();
    reverse(res.begin(), res.end());
    return res;
}
 
/*
MUL
- a * b
- O(n^2)
- استخدام: ضرب أرقام كبيرة (≤ ~2000 digit)
*/
string mul(string a, string b) {
    int n = a.size(), m = b.size();
    int arr[5005] = {0};
 
    for (int i = n - 1; i >= 0; i--) {
        for (int j = m - 1; j >= 0; j--) {
            int x = (a[i] - '0') * (b[j] - '0');
            int p1 = i + j, p2 = i + j + 1;
            int sum = x + arr[p2];
 
            arr[p2] = sum % 10;
            arr[p1] += sum / 10;
        }
    }
 
    string res = "";
    for (int i = 0; i < n + m; i++) {
        if (!(res.empty() && arr[i] == 0)) {
            res += char(arr[i] + '0');
        }
    }
 
    return res.empty() ? "0" : res;
}
 
/*
MUL_DIGIT (Optimization)
- a * digit (0→9)
- O(n)
- مهم جدًا في division
*/
string mul_digit(string a, int d) {
    if (d == 0) return "0";
    if (d == 1) return a;
 
    string res = "";
    int carry = 0;
 
    for (int i = a.size() - 1; i >= 0; i--) {
        int x = (a[i] - '0') * d + carry;
        res += char(x % 10 + '0');
        carry = x / 10;
    }
 
    if (carry) res += char(carry + '0');
    reverse(res.begin(), res.end());
    return res;
}
 
/*
DIV (OPTIMIZED LONG DIVISION)
- a / b
- O(n^2)
- استخدام: لما تحتاج division فعلاً
- أسرع بكتير من naive
*/
string divv(string a, string b) {
    string cur = "";
    string res = "";
 
    for (int i = 0; i < a.size(); i++) {
        cur += a[i];
 
        int l = 0, r = 9, best = 0;
 
        // binary search على digit
        while (l <= r) {
            int mid = (l + r) / 2;
            string t = mul_digit(b, mid);
 
            if (!smaller(cur, t)) {
                best = mid;
                l = mid + 1;
            } else r = mid - 1;
        }
 
        res += char(best + '0');
        cur = sub(cur, mul_digit(b, best));
    }
 
    int i = 0;
    while (i < res.size() - 1 && res[i] == '0') i++;
    return res.substr(i);
}
 
/*
MOD (OPTIMIZED)
- a % b
- O(n^2)
- استخدام: لما تحتاج remainder
*/
string mod(string a, string b) {
    string cur = "";
 
    for (int i = 0; i < a.size(); i++) {
        cur += a[i];
 
        int l = 0, r = 9, best = 0;
 
        while (l <= r) {
            int mid = (l + r) / 2;
            string t = mul_digit(b, mid);
 
            if (!smaller(cur, t)) {
                best = mid;
                l = mid + 1;
            } else r = mid - 1;
        }
 
        cur = sub(cur, mul_digit(b, best));
    }
 
    return cur;
}
