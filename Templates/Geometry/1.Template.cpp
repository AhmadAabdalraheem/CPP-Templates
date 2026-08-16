#include <bits/stdc++.h>
using namespace std;

/*
 * =============================================================================
 * COMPREHENSIVE 2D GEOMETRY TEMPLATE FOR COMPETITIVE PROGRAMMING
 * =============================================================================
 * 
 * PROBLEM TYPES & TRIGGER CRITERIA:
 * - Point, Vector, Line, Segment, Polygon, and Circle manipulation.
 * - Intersection problems (Line-Line, Segment-Segment, Circle-Circle, Line-Circle).
 * - Convex Hull, Point-in-Polygon (O(N) general, O(log N) convex).
 * - Half-Plane Intersection O(N log N) for Feasible Regions / Kernel.
 * - Rotating Calipers O(N) for Diameter / Farthest Pair of Points.
 * - Minimum Enclosing Circle (Welzl's Algorithm) in Expected O(N).
 * - Tangents (Point-to-Circle, Circle-to-Circle).
 * - Minkowski Sum of Convex Polygons, Closest Pair of Points, Polygon Clipping.
 *-
 * CORE IDEA & MODIFICATIONS:
 * - Floating-point precision managed via EPS (1e-9).
 * - All structural objects utilize custom epsilon-safe comparisons (sgn, eq, le, ge).
 * - Replace `long double` with `long long` for exact integer geometry when applicable.
 * =============================================================================
 */

typedef long double ld;
typedef ld T;

const ld EPS = 1e-9;
const ld PI = acosl(-1.0);

/*
  ============================================================================
  1. PRECISION & SIGN HELPER FUNCTIONS
  ============================================================================
*/
int sgn(ld val) {
    if (val < -EPS) return -1;
    if (val > EPS) return 1;
    return 0;
}

bool eq(ld a, ld b) { return fabsl(a - b) < EPS; }
bool le(ld a, ld b) { return a < b + EPS; }
bool ge(ld a, ld b) { return a > b - EPS; }

/*
  ============================================================================
  2. POINT & VECTOR STRUCTURE
  ============================================================================
*/
struct pt {
    T x, y;

    pt operator+(pt p) const { return {x + p.x, y + p.y}; }
    pt operator-(pt p) const { return {x - p.x, y - p.y}; }
    pt operator*(T d) const { return {x * d, y * d}; }
    pt operator/(T d) const { return {x / d, y / d}; }

    bool operator==(pt p) const { return eq(x, p.x) && eq(y, p.y); }
    bool operator!=(pt p) const { return !(*this == p); }
    bool operator<(pt p) const {
        if (!eq(x, p.x)) return x < p.x;
        return y < p.y - EPS;
    }
};

void takepoint(pt &p) {
    T x, y;
    cin >> x >> y;
    p = {x, y};
}

T sq(pt p) { return p.x * p.x + p.y * p.y; }
ld length(pt p) { return sqrtl(sq(p)); }

/*
  ============================================================================
  3. VECTOR TRANSFORMATIONS
  ============================================================================
*/
pt translate(pt v, pt p) { return p + v; }
pt scale(pt c, ld factor, pt p) { return c + (p - c) * factor; }

pt rot(pt p, ld a) {
    return {
        p.x * cosl(a) - p.y * sinl(a),
        p.x * sinl(a) + p.y * cosl(a)
    };
}

pt rot(pt p, pt c, ld a) { return c + rot(p - c, a); }
pt perp(pt p) { return {-p.y, p.x}; }

/*
  ============================================================================
  4. VECTOR PRODUCTS & ANGLES
  ============================================================================
*/
T dot(pt a, pt b) { return a.x * b.x + a.y * b.y; }
T cross(pt a, pt b) { return a.x * b.y - a.y * b.x; }

ld angle(pt a, pt b) {
    ld cos_theta = dot(a, b) / sqrtl(sq(a) * sq(b));
    return acosl(max((ld)-1.0, min((ld)1.0, cos_theta)));
}

T orient(pt a, pt b, pt c) { return cross(b - a, c - a); }
bool isPerpendicular(pt a, pt b) { return fabsl(dot(a, b)) < EPS; }

ld orientedAngle(pt a, pt b, pt c) {
    ld ang = angle(b - a, c - a);
    if (orient(a, b, c) >= 0) return ang;
    return 2.0 * PI - ang;
}

bool inAngle(pt a, pt b, pt c, pt p) {
    T abp = orient(a, b, p);
    T acp = orient(a, c, p);
    T abc = orient(a, b, c);
    if (abc < 0) swap(abp, acp);
    return (abp >= 0 && acp <= 0);
}

/*
  ============================================================================
  5. POLAR SORTING (ANGLE SORT)
  ============================================================================
*/
int half(pt p) {
    return p.y > 0 || (eq(p.y, 0) && p.x > 0) ? 0 : 1;
}

bool polarComp(pt a, pt b) {
    int h1 = half(a), h2 = half(b);
    if (h1 != h2) return h1 < h2;
    T c = cross(a, b);
    if (sgn(c) != 0) return c > 0;
    return sq(a) < sq(b);
}

/*
  ============================================================================
  6. LINE STRUCTURE & LINE OPERATIONS
  ============================================================================
*/
struct line {
    pt v; T c;

    line(T a, T b, T _c) {
        v = {b, -a};
        c = _c;
    }

    line(pt p, pt q) {
        v = q - p;
        c = cross(v, p);
    }

    T side(pt p) const { return cross(v, p) - c; }
    ld dist(pt p) const { return fabsl(side(p)) / sqrtl(sq(v)); }
    pt proj(pt p) const { return p - perp(v) * side(p) / sq(v); }
    pt refl(pt p) const { return p - perp(v) * 2.0 * side(p) / sq(v); }
};

bool inter(line l1, line l2, pt &out) {
    T d = cross(l1.v, l2.v);
    if (fabsl(d) < EPS) return false;
    out = (l2.v * l1.c - l1.v * l2.c) / d;
    return true;
}

line bisector(line l1, line l2, bool interior = true) {
    ld sign = interior ? 1.0 : -1.0;
    ld len1 = sqrtl(sq(l1.v));
    ld len2 = sqrtl(sq(l2.v));
    return {
        l2.v / len2 + l1.v / len1 * sign,
        l2.c / len2 + l1.c / len1 * sign
    };
}

/*
  ============================================================================
  7. SEGMENT OPERATIONS
  ============================================================================
*/
bool onSegment(pt a, pt b, pt p) {
    return sgn(orient(a, b, p)) == 0 && sgn(dot(a - p, b - p)) <= 0;
}

bool segInter(pt a, pt b, pt c, pt d, pt &out) {
    T oa = orient(c, d, a), ob = orient(c, d, b);
    T oc = orient(a, b, c), od = orient(a, b, d);
    
    if (sgn(oa) * sgn(ob) < 0 && sgn(oc) * sgn(od) < 0) {
        out = (a * ob - b * oa) / (ob - oa);
        return true;
    }
    if (onSegment(c, d, a)) { out = a; return true; }
    if (onSegment(c, d, b)) { out = b; return true; }
    if (onSegment(a, b, c)) { out = c; return true; }
    if (onSegment(a, b, d)) { out = d; return true; }
    return false;
}

ld distToSegment(pt a, pt b, pt p) {
    if (a == b) return length(p - a);
    pt v = b - a;
    T u = dot(p - a, v) / sq(v);
    if (u < 0) return length(p - a);
    if (u > 1) return length(p - b);
    return length(p - (a + v * u));
}

/*
  ============================================================================
  8. POLYGON OPERATIONS
  ============================================================================
*/
ld polygonArea(const vector<pt>& p) {
    T area = 0;
    int n = p.size();
    for (int i = 0; i < n; i++) {
        area += cross(p[i], p[(i + 1) % n]);
    }
    return fabsl(area) / 2.0;
}

// Ray-Casting Algorithm O(N) - General Polygon
// Returns: 1 inside, 0 on boundary, -1 outside
int inPolygon(const vector<pt>& p, pt pt_in) {
    int n = p.size();
    bool inside = false;
    for (int i = 0; i < n; i++) {
        pt a = p[i], b = p[(i + 1) % n];
        if (onSegment(a, b, pt_in)) return 0;
        if ((a.y > pt_in.y) != (b.y > pt_in.y)) {
            ld x_inter = (b.x - a.x) * (pt_in.y - a.y) / (b.y - a.y) + a.x;
            if (pt_in.x < x_inter) inside = !inside;
        }
    }
    return inside ? 1 : -1;
}

// Binary Search O(log N) - CCW Convex Polygon Only
// Returns: 1 inside, 0 on boundary, -1 outside
int inConvexPolygon(const vector<pt>& p, pt q) {
    int n = p.size();
    if (n < 3) return -1;
    
    T o1 = orient(p[0], p[1], q);
    T o2 = orient(p[0], p[n - 1], q);
    if (o1 < 0 || o2 > 0) return -1;
    if (o1 == 0 && onSegment(p[0], p[1], q)) return 0;
    if (o2 == 0 && onSegment(p[0], p[n - 1], q)) return 0;

    int l = 1, r = n - 1;
    while (r - l > 1) {
        int mid = l + (r - l) / 2;
        if (orient(p[0], p[mid], q) >= 0) l = mid;
        else r = mid;
    }
    
    T in_wedge = orient(p[l], p[r], q);
    if (in_wedge < 0) return -1;
    if (in_wedge == 0) return 0;
    return 1;
}

bool isConvex(const vector<pt>& p) {
    int n = p.size();
    if (n < 3) return false;
    bool hasPos = false, hasNeg = false;
    for (int i = 0; i < n; i++) {
        T o = orient(p[i], p[(i + 1) % n], p[(i + 2) % n]);
        if (o > 0) hasPos = true;
        if (o < 0) hasNeg = true;
    }
    return !(hasPos && hasNeg);
}

// Andrew's Monotone Chain - O(N log N)
vector<pt> convexHull(vector<pt> pts) {
    int n = pts.size();
    if (n <= 2) return pts;
    sort(pts.begin(), pts.end());
    vector<pt> hull;
    for (int i = 0; i < n; i++) {
        while (hull.size() >= 2 && sgn(orient(hull[hull.size() - 2], hull.back(), pts[i])) <= 0)
            hull.pop_back();
        hull.push_back(pts[i]);
    }
    for (int i = n - 2, t = hull.size() + 1; i >= 0; i--) {
        while ((int)hull.size() >= t && sgn(orient(hull[hull.size() - 2], hull.back(), pts[i])) <= 0)
            hull.pop_back();
        hull.push_back(pts[i]);
    }
    hull.pop_back();
    return hull;
}

// Rotating Calipers O(N) - Convex Hull Diameter Squared
T convexHullDiameterSq(const vector<pt>& hull) {
    int n = hull.size();
    if (n <= 1) return 0;
    if (n == 2) return sq(hull[0] - hull[1]);

    T max_d = 0;
    int k = 1;
    while (orient(hull[n - 1], hull[0], hull[(k + 1) % n]) >
           orient(hull[n - 1], hull[0], hull[k])) {
        k++;
    }

    for (int i = 0, j = k; i <= k; i++) {
        while (orient(hull[i], hull[(i + 1) % n], hull[(j + 1) % n]) >
               orient(hull[i], hull[(i + 1) % n], hull[j])) {
            max_d = max(max_d, sq(hull[i] - hull[j]));
            j = (j + 1) % n;
        }
        max_d = max(max_d, sq(hull[i] - hull[j]));
    }
    return max_d;
}

// Polygon Cut / Clipping by Line (Keeps part on the left side of line l)
vector<pt> cutPolygon(const vector<pt>& p, line l) {
    vector<pt> res;
    int n = p.size();
    for (int i = 0; i < n; i++) {
        pt cur = p[i], next = p[(i + 1) % n];
        T s1 = l.side(cur), s2 = l.side(next);
        if (s1 >= -EPS) res.push_back(cur);
        if (sgn(s1) * sgn(s2) < 0) {
            pt out;
            inter(l, line(cur, next), out);
            res.push_back(out);
        }
    }
    return res;
}

/*
  ============================================================================
  9. CIRCLE STRUCTURE & CIRCLE OPERATIONS
  ============================================================================
*/
struct circle {
    pt c; ld r;
};

vector<pt> interLineCircle(line l, circle circ) {
    vector<pt> res;
    ld d = l.dist(circ.c);
    if (sgn(d - circ.r) > 0) return res;
    pt proj = l.proj(circ.c);
    if (sgn(d - circ.r) == 0) {
        res.push_back(proj);
        return res;
    }
    ld h = sqrtl(max((ld)0.0, circ.r * circ.r - d * d));
    pt v = l.v / length(l.v);
    res.push_back(proj + v * h);
    res.push_back(proj - v * h);
    return res;
}

vector<pt> interCircleCircle(circle c1, circle c2) {
    vector<pt> res;
    ld d = length(c1.c - c2.c);
    if (sgn(d) == 0 || sgn(d - (c1.r + c2.r)) > 0 || sgn(d - fabsl(c1.r - c2.r)) < 0)
        return res;
    ld a = (c1.r * c1.r - c2.r * c2.r + d * d) / (2.0 * d);
    ld h = sqrtl(max((ld)0.0, c1.r * c1.r - a * a));
    pt p2 = c1.c + (c2.c - c1.c) * (a / d);
    if (sgn(h) == 0) {
        res.push_back(p2);
    } else {
        pt v = perp(c2.c - c1.c) * (h / d);
        res.push_back(p2 + v);
        res.push_back(p2 - v);
    }
    return res;
}

vector<pt> tangents(pt p, circle circ) {
    return interCircleCircle(circ, circle{p, sqrtl(sq(p - circ.c) - circ.r * circ.r)});
}

// Common Tangents between Two Circles
vector<line> commonTangents(circle c1, circle c2) {
    vector<line> ans;
    for (int inner : {-1, 1}) {
        ld r2 = c2.r * inner;
        ld dr = c1.r - r2;
        pt d = c2.c - c1.c;
        ld dist_sq = sq(d);
        if (sgn(dist_sq - dr * dr) < 0) continue;
        ld h = sqrtl(max((ld)0.0, dist_sq - dr * dr));
        
        for (int sign : {-1, 1}) {
            pt v = (d * dr + perp(d) * (h * sign)) / dist_sq;
            pt p1 = c1.c + v * c1.r;
            pt p2 = c2.c + v * r2;
            if (p1 == p2) {
                ans.push_back(line(p1, p1 + perp(d)));
            } else {
                ans.push_back(line(p1, p2));
            }
        }
    }
    return ans;
}

circle circumcircle(pt a, pt b, pt c) {
    line perp1((a + b) / 2.0, (a + b) / 2.0 + perp(b - a));
    line perp2((b + c) / 2.0, (b + c) / 2.0 + perp(c - b));
    pt center;
    inter(perp1, perp2, center);
    return {center, length(a - center)};
}

circle incircle(pt a, pt b, pt c) {
    ld la = length(b - c), lb = length(a - c), lc = length(a - b);
    pt center = (a * la + b * lb + c * lc) / (la + lb + lc);
    line l(a, b);
    return {center, l.dist(center)};
}

// Welzl's Algorithm for Minimum Enclosing Circle - Expected O(N)
circle mecTwoPoints(pt a, pt b) {
    return {(a + b) / 2.0, length(a - b) / 2.0};
}

circle minEnclosingCircle(vector<pt> pts) {
    static mt19937 rng(1337);
    shuffle(pts.begin(), pts.end(), rng);

    int n = pts.size();
    if (n == 0) return {{0, 0}, 0};
    if (n == 1) return {pts[0], 0};

    circle c = mecTwoPoints(pts[0], pts[1]);
    for (int i = 2; i < n; i++) {
        if (length(pts[i] - c.c) > c.r + EPS) {
            c = {pts[i], 0};
            for (int j = 0; j < i; j++) {
                if (length(pts[j] - c.c) > c.r + EPS) {
                    c = mecTwoPoints(pts[i], pts[j]);
                    for (int k = 0; k < j; k++) {
                        if (length(pts[k] - c.c) > c.r + EPS) {
                            c = circumcircle(pts[i], pts[j], pts[k]);
                        }
                    }
                }
            }
        }
    }
    return c;
}

/*
  ============================================================================
  10. HALF-PLANE INTERSECTION O(N log N)
  ============================================================================
*/
struct Halfplane {
    pt p, v;
    ld angle;

    Halfplane() {}
    Halfplane(pt p, pt q) : p(p), v(q - p), angle(atan2l(v.y, v.x)) {}

    bool out(pt q) const { return orient(p, p + v, q) < -EPS; }

    bool operator<(const Halfplane& hp) const {
        if (!eq(angle, hp.angle)) return angle < hp.angle;
        return orient(p, p + v, hp.p) < 0;
    }
};

vector<pt> halfplaneIntersection(vector<Halfplane> h) {
    sort(h.begin(), h.end());

    vector<Halfplane> keep;
    for (int i = 0; i < (int)h.size(); i++) {
        if (i && eq(h[i].angle, h[i - 1].angle)) continue;
        keep.push_back(h[i]);
    }

    deque<Halfplane> dq;
    deque<pt> pts;

    for (auto& hp : keep) {
        while (pts.size() && hp.out(pts.back())) {
            pts.pop_back();
            dq.pop_back();
        }
        while (pts.size() && hp.out(pts.front())) {
            pts.pop_front();
            dq.pop_front();
        }
        pt inter_pt;
        if (dq.size()) {
            inter(line(dq.back().p, dq.back().p + dq.back().v), line(hp.p, hp.p + hp.v), inter_pt);
            pts.push_back(inter_pt);
        }
        dq.push_back(hp);
    }

    while (pts.size() && dq.front().out(pts.back())) {
        pts.pop_back();
        dq.pop_back();
    }
    while (pts.size() && dq.back().out(pts.front())) {
        pts.pop_front();
        dq.pop_front();
    }

    if (dq.size() < 3) return {};

    vector<pt> poly(pts.begin(), pts.end());
    pt final_pt;
    inter(line(dq.back().p, dq.back().p + dq.back().v), line(dq.front().p, dq.front().p + dq.front().v), final_pt);
    poly.push_back(final_pt);
    return poly;
}

/*
  ============================================================================
  11. ADVANCED GEOMETRY ALGORITHMS
  ============================================================================
*/

// Minkowski Sum of two Convex Polygons O(N + M)
void reorder_polygon(vector<pt>& p) {
    size_t pos = 0;
    for (size_t i = 1; i < p.size(); i++) {
        if (p[i].y < p[pos].y || (eq(p[i].y, p[pos].y) && p[i].x < p[pos].x))
            pos = i;
    }
    rotate(p.begin(), p.begin() + pos, p.end());
}

vector<pt> minkowskiSum(vector<pt> P, vector<pt> Q) {
    reorder_polygon(P);
    reorder_polygon(Q);
    P.push_back(P[0]);
    P.push_back(P[1]);
    Q.push_back(Q[0]);
    Q.push_back(Q[1]);
    vector<pt> res;
    size_t i = 0, j = 0;
    while (i < P.size() - 2 || j < Q.size() - 2) {
        res.push_back(P[i] + Q[j]);
        T c = cross(P[i + 1] - P[i], Q[j + 1] - Q[j]);
        if (c >= 0 && i < P.size() - 2) i++;
        if (c <= 0 && j < Q.size() - 2) j++;
    }
    return res;
}

// Closest Pair of Points Sweep-Line O(N log N)
ld closestPair(vector<pt> pts) {
    int n = pts.size();
    if (n <= 1) return 1e18;
    sort(pts.begin(), pts.end(), [](pt a, pt b) { return a.x < b.x; });
    auto cmpY = [](pt a, pt b) { return a.y < b.y; };
    multiset<pt, decltype(cmpY)> s(cmpY);
    ld d = 1e18;
    int left = 0;
    for (int i = 0; i < n; i++) {
        while (left < i && pts[i].x - pts[left].x >= d) {
            s.erase(s.find(pts[left]));
            left++;
        }
        auto it1 = s.lower_bound({pts[i].x, pts[i].y - d});
        auto it2 = s.upper_bound({pts[i].x, pts[i].y + d});
        for (auto it = it1; it != it2; ++it) {
            d = min(d, length(pts[i] - *it));
        }
        s.insert(pts[i]);
    }
    return d;
}

// Pick's Theorem Helper (Requires integer coordinates)
// Area = I + B/2 - 1  ==>  I = Area - B/2 + 1
// Returns pair: {boundary_lattice_points (B), interior_lattice_points (I)}
pair<long long, long long> latticePointsCount(const vector<pt>& p) {
    int n = p.size();
    long long B = 0;
    for (int i = 0; i < n; i++) {
        pt a = p[i], b = p[(i + 1) % n];
        long long dx = llround(fabsl(a.x - b.x));
        long long dy = llround(fabsl(a.y - b.y));
        B += std::gcd(dx, dy);
    }
    ld double_area = 0;
    for (int i = 0; i < n; i++) {
        double_area += cross(p[i], p[(i + 1) % n]);
    }
    long long twice_area = llround(fabsl(double_area));
    long long I = (twice_area - B + 2) / 2;
    return {B, I};
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cout << fixed << setprecision(6);
    // ============================================================================
    // 2. POINT & VECTOR STRUCTURE
    // ============================================================================
    cout << "=== 2. Point & Vector Basics ===\n";
    pt a = {0, 0}, b = {4, 0}, c = {2, 3};
    pt v = b - a; // Vector displacement
    cout << "Vector b: (" << b.x << ", " << b.y << ")\n";
    cout << "Length of b: " << length(b) << "\n";
    cout << "Squared length of c (x^2 + y^2): " << sq(c) << "\n\n";

    // ============================================================================
    // 3. VECTOR TRANSFORMATIONS
    // ============================================================================
    cout << "=== 3. Transformations ===\n";
    pt b_rot = rot(b, PI / 2.0); // Rotate (4,0) 90 deg CCW -> (0,4)
    cout << "Point b rotated 90 deg CCW: (" << b_rot.x << ", " << b_rot.y << ")\n";
    pt b_perp = perp(b); // Perpendicular vector (-y, x) -> (0,4)
    cout << "Perpendicular of b: (" << b_perp.x << ", " << b_perp.y << ")\n";
    pt b_scaled = scale(a, 2.0, b); // Scale b relative to origin by factor 2 -> (8,0)
    cout << "Point b scaled by 2 from origin: (" << b_scaled.x << ", " << b_scaled.y << ")\n\n";

    // ============================================================================
    // 4. VECTOR PRODUCTS & ANGLES
    // ============================================================================
    cout << "=== 4. Products & Angles ===\n";
    cout << "Dot Product (a->b . a->c): " << dot(b - a, c - a) << "\n";
    cout << "Cross Product (a->b x a->c): " << cross(b - a, c - a) << "\n";
    T ori = orient(a, b, c);
    cout << "Orientation of C relative to A->B: " 
         << (ori > 0 ? "LEFT (CCW)" : (ori < 0 ? "RIGHT (CW)" : "COLLINEAR")) << "\n";
    cout << "Angle between A->B and A->C (rad): " << angle(b - a, c - a) << "\n\n";

    // ============================================================================
    // 5. POLAR SORTING (ANGLE SORT)
    // ============================================================================
    cout << "=== 5. Polar Sorting ===\n";
    vector<pt> pts_to_sort = {{1, 1}, {-1, 1}, {-1, -1}, {1, -1}, {1, 0}};
    sort(pts_to_sort.begin(), pts_to_sort.end(), polarComp);
    cout << "Points sorted CCW around origin:\n";
    for (auto p : pts_to_sort) cout << "(" << p.x << ", " << p.y << ") ";
    cout << "\n\n";

    // ============================================================================
    // 6. LINE STRUCTURE & OPERATIONS
    // ============================================================================
    cout << "=== 6. Line Operations ===\n";
    line l1(a, b); // Horizontal line y = 0
    line l2(pt{2, 0}, pt{2, 4}); // Vertical line x = 2
    pt p_test = {2, 5};

    cout << "Distance from P(2,5) to line AB: " << l1.dist(p_test) << "\n";
    pt proj_p = l1.proj(p_test);
    cout << "Projection of P(2,5) on line AB: (" << proj_p.x << ", " << proj_p.y << ")\n";
    pt refl_p = l1.refl(p_test);
    cout << "Reflection of P(2,5) across line AB: (" << refl_p.x << ", " << refl_p.y << ")\n";

    pt inter_pt;
    if (inter(l1, l2, inter_pt)) {
        cout << "Intersection of Line 1 and Line 2: (" << inter_pt.x << ", " << inter_pt.y << ")\n\n";
    }

    // ============================================================================
    // 7. SEGMENT OPERATIONS
    // ============================================================================
    cout << "=== 7. Segment Operations ===\n";
    pt seg1_a = {0, 0}, seg1_b = {4, 4};
    pt seg2_a = {0, 4}, seg2_b = {4, 0};
    pt seg_inter_pt;

    if (segInter(seg1_a, seg1_b, seg2_a, seg2_b, seg_inter_pt)) {
        cout << "Segment intersection: (" << seg_inter_pt.x << ", " << seg_inter_pt.y << ")\n";
    }
    cout << "Dist from (-1,0) to segment (0,0)->(4,0): " << distToSegment(a, b, pt{-1, 0}) << "\n";
    cout << "Is (2,0) on segment (0,0)->(4,0)? " << (onSegment(a, b, {2, 0}) ? "YES" : "NO") << "\n\n";

    // ============================================================================
    // 8. POLYGON OPERATIONS
    // ============================================================================
    cout << "=== 8. Polygon Operations ===\n";
    vector<pt> poly = {{0, 0}, {4, 0}, {4, 3}, {0, 3}}; // Rectangle 4x3
    cout << "Polygon Area: " << polygonArea(poly) << "\n";
    cout << "Is Point (2,1) in Polygon? " << inPolygon(poly, {2, 1}) << " (1: In, 0: On, -1: Out)\n";
    cout << "Is Polygon Convex? " << (isConvex(poly) ? "YES" : "NO") << "\n";

    // Convex Hull Example
    vector<pt> cloud = {{0, 0}, {4, 0}, {2, 1}, {4, 3}, {0, 3}, {2, 2}};
    vector<pt> hull = convexHull(cloud);
    cout << "Convex Hull Vertices:\n";
    for (auto p : hull) cout << "(" << p.x << ", " << p.y << ") ";
    cout << "\n\n";

    // ============================================================================
    // 9. CIRCLE OPERATIONS
    // ============================================================================
    cout << "=== 9. Circle Operations ===\n";
    circle c1 = {{0, 0}, 5.0};
    circle c2 = {{3, 0}, 4.0};

    // Circle-Circle Intersections
    vector<pt> circle_inters = interCircleCircle(c1, c2);
    cout << "Intersections between C1 and C2:\n";
    for (auto p : circle_inters) cout << "(" << p.x << ", " << p.y << ") ";
    cout << "\n";

    // Circumcircle & Incircle of Triangle ABC
    circle circum = circumcircle(a, b, c);
    cout << "Circumcenter of Triangle ABC: (" << circum.c.x << ", " << circum.c.y 
         << "), Radius: " << circum.r << "\n";

    circle inc = incircle(a, b, c);
    cout << "Incenter of Triangle ABC: (" << inc.c.x << ", " << inc.c.y 
         << "), Radius: " << inc.r << "\n";

    // Tangents from point (0, 10) to Circle C1
    vector<pt> tang_pts = tangents({0, 10}, c1);
    cout << "Tangent points on C1 from (0,10):\n";
    for (auto p : tang_pts) cout << "(" << p.x << ", " << p.y << ") ";
    cout << "\n";

    return 0;
}
