#include <bits/stdc++.h>
using namespace std;

typedef long double ld;
typedef ld T;

// point structure
struct pt {
    T x, y;

    // vector addition
    pt operator+(pt p){ return {x + p.x, y + p.y}; }

    // vector subtraction
    pt operator-(pt p){ return {x - p.x, y - p.y}; }

    // multiply by scalar
    pt operator*(T d){ return {x * d, y * d}; }

    // divide by scalar
    pt operator/(T d){ return {x / d, y / d}; }
};

// read integer point and convert
void takepoint(pt &p){
    int x, y;
    cin >> x >> y;
    p = {x, y};
}

// squared distance from origin
T sq(pt p){
    return p.x * p.x + p.y * p.y;
}

// length of vector
ld length(pt p){
    return sqrtl(sq(p));
}
