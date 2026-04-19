include <bits/stdc++.h>
using namespace std;

typedef long double ld;
typedef ld T;
const ld EPS = 1e-9;

struct pt{
    T x, y;
    pt operator+(pt p){ return {x + p.x, y + p.y}; }
    pt operator-(pt p){ return {x - p.x, y - p.y}; }
    pt operator*(T d){ return {x * d, y * d}; }
};

// cross product
T cross(pt a, pt b){
    return a.x * b.y - a.y * b.x;
}

// dot product
T dot(pt a, pt b){
    return a.x * b.x + a.y * b.y;
}

// squared length
T sq(pt p){
    return dot(p,p);
}

// perpendicular vector
pt perp(pt p){
    return {-p.y, p.x};
}

// line representation
struct line{
    pt v; T c; // direction vector + constant

    // from ax + by = c
    line(T a, T b, T _c){
        v = {b, -a};
        c = _c;
    }

    // from two points
    line(pt p, pt q){
        v = q - p;
        c = cross(v, p);
    }

    // which side of line
    T side(pt p){
        return cross(v, p) - c;
    }

    // distance from point to line
    ld dist(pt p){
        return fabsl(side(p)) / sqrtl(sq(v));
    }

    // projection of point on line
    pt proj(pt p){
        return p - perp(v) * side(p) / sq(v);
    }

    // reflection of point across line
    pt refl(pt p){
        return p - perp(v) * 2.0 * side(p) / sq(v);
    }
};

// intersection of two lines
bool inter(line l1, line l2, pt &out){
    T d = cross(l1.v, l2.v);
    if(fabsl(d) < EPS) return false;
    out = (l2.v * l1.c - l1.v * l2.c) / d;
    return true;
}

// angle bisector
line bisector(line l1, line l2, bool interior){
    ld sign = interior ? 1 : -1;
    return {
        l2.v / sqrtl(sq(l2.v)) + l1.v / sqrtl(sq(l1.v)) * sign,
        l2.c / sqrtl(sq(l2.v)) + l1.c / sqrtl(sq(l1.v)) * sign
    };
}
