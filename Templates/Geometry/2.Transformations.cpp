#include <bits/stdc++.h>
using namespace std;

typedef long double ld;
typedef ld T;

struct pt{
    T x, y;
    pt operator+(pt p){ return {x + p.x, y + p.y}; }
    pt operator-(pt p){ return {x - p.x, y - p.y}; }
    pt operator*(T d){ return {x * d, y * d}; }
};

// translate point by vector v
pt translate(pt v, pt p){
    return p + v;
}

// scale point relative to center c
pt scale(pt c, ld factor, pt p){
    pt v = p - c;
    return c + v * factor;
}

// rotate point around origin
pt rot(pt p, ld a){
    return {
        p.x * cos(a) - p.y * sin(a),
        p.x * sin(a) + p.y * cos(a)
    };
}

// rotate point around center c
pt rot(pt p, pt c, ld a){
    return c + rot(p - c, a);
}

// perpendicular vector (90 degrees CCW)
pt perp(pt p){
    return {-p.y, p.x};
}
