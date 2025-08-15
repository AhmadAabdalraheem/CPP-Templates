struct line{
    pt v; T c; // vector , constant
 
    line(pt v, T c) : v(v), c(c) {}
 
    // from equation ax+by = c
    line(T a, T b, T _c){
        v = {b, -a};
        c = _c;
    }
 
    //line from two points
    line(pt p, pt q){
        v = q - p;
        c = cross(v, p);
    }
 
    T side(pt p) {return cross(v,p)-c;} // + L ,  - R 
    ld dist(pt p) {return abs(side(p)) / abs(v);} // dist from point to line "perp"
    double sqDist(pt p) {return side(p)*side(p) / (T)sq(v);}
    line prepThrought(pt p){ return {p, p + prep(p)};} // perp to line , pass throug p 
    bool cmpProj(pt p, pt q){ //sorting to line 
        return dot(v, p) < dot(v, q);
    }
    line translate(pt t) {return {v, c + cross(v,t)};}
    line shiftLeft(T dist) {return {v, c + dist*abs(v)};}
    pt proj(pt p) {return p - prep(v)*side(p)/sq(v);}
    pt refl(pt p) {return p - prep(v) * (T)2.0 * side(p)/sq(v);}
};
bool inter(line l1, line l2, pt &out) {
    T d = cross(l1.v, l2.v);
    if (fabs(d) < EPS) return false;
    out = (l2.v*l1.c- l1.v*l2.c) / d; // requires floating-point coordinates
    return true;
}
 
line bisector(line l1, line l2, bool interior) { // خط بينصف الزاوية بين خطين
  // interior = 1 --> اللي بينهم
  // interior = 0 --> الخارجي
    assert(cross(l1.v, l2.v) != 0); // l1 and l2 cannot be parallel!
    ld sign = interior ? 1 :-1;
    return {l2.v/abs(l2.v) + l1.v/abs(l1.v) * sign,
            l2.c/abs(l2.v) + l1.c/abs(l1.v) * sign};
}
 
