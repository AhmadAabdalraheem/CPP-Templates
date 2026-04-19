#include <bits/stdc++.h>
using namespace std;

typedef long double ld;
typedef ld T;
const ld EPS = 1e-9;

struct pt{
    T x, y;
    pt operator+(pt p){ return {x + p.x, y + p.y}; }
    pt operator-(pt p){ return {x - p.x, y - p.y}; }
};

// dot product
T dot(pt a, pt b){
    return a.x * b.x + a.y * b.y;
}

// cross product
T cross(pt a, pt b){
    return a.x * b.y - a.y * b.x;
}

// angle between two vectors (small angle)
ld angle(pt a, pt b){
    return acosl(max((ld)-1.0, min((ld)1.0, dot(a,b)/sqrtl(dot(a,a)*dot(b,b)))));
}

// orientation (left/right)
T orient(pt a, pt b, pt c){
    return cross(b - a, c - a);
}

// check perpendicular
bool isPerpendicular(pt a, pt b){
    return fabsl(dot(a, b)) < EPS;
}

// oriented angle (counter-clockwise)
ld orientedAngle(pt a, pt b, pt c){
    ld ang = angle(b - a, c - a);
    if(orient(a,b,c) > 0) return ang;
    return 2 * M_PI - ang;
}

// check if point p inside angle BAC
bool inAngle(pt a, pt b, pt c, pt p){
    T abp = orient(a,b,p);
    T acp = orient(a,c,p);
    T abc = orient(a,b,c);
    if(abc < 0) swap(abp, acp);
    return (abp >= 0 && acp <= 0);
}
