 * =============================================================================
 *                    GEOMETRY — PROBLEM INDEX / CHEAT SHEET
 * =============================================================================
 *
 *  1. POINTS & VECTORS
 *     - Point / Vector operations
 *     - Distance / Length
 *     - Translation / Scaling / Rotation
 *     - Perpendicular vectors
 *
 *  2. DOT & CROSS PRODUCT
 *     - Perpendicularity
 *     - Angle between vectors
 *     - Orientation / Clockwise / Counter-clockwise
 *     - Collinearity
 *     - Left / Right side of a line
 *
 *  3. POLAR / ANGULAR SORTING
 *     - Sort points / vectors by angle
 *     - Clockwise / Counter-clockwise ordering
 *     - Angular sweep / visibility problems
 *
 *  4. LINES
 *     - Line construction
 *     - Point-to-line distance
 *     - Projection
 *     - Reflection
 *     - Line intersection
 *     - Angle bisectors
 *
 *  5. SEGMENTS
 *     - Point on segment
 *     - Segment intersection
 *     - Point-to-segment distance
 *     - Crossing / touching segments
 *
 *  6. INTERSECTION PROBLEMS
 *     - Line × Line
 *     - Segment × Segment
 *     - Line × Circle
 *     - Circle × Circle
 *
 *  7. POLYGON BASICS
 *     - Polygon area
 *     - Point inside polygon
 *     - Convexity test
 *     - Convex Hull
 *     - Polygon clipping / cutting
 *
 *  8. POINT IN POLYGON
 *     - General Polygon       → O(N)
 *     - Convex Polygon        → O(log N)
 *
 *  9. CONVEX HULL
 *     - Enclose a set of points
 *     - Outer boundary
 *     - Remove interior points
 *     - Andrew's Monotone Chain → O(N log N)
 *
 * 10. CONVEX HULL + ROTATING CALIPERS
 *     - Farthest pair of points
 *     - Polygon diameter
 *     - Maximum distance
 *     - O(N) after Convex Hull
 *
 * 11. CIRCLES
 *     - Line × Circle intersection
 *     - Circle × Circle intersection
 *     - Tangency checks
 *     - Circumcircle
 *     - Incircle
 *
 * 12. TANGENTS
 *     - Tangents from Point → Circle
 *     - Common Tangents between two Circles
 *     - External / Internal tangents
 *
 * 13. TRIANGLE GEOMETRY
 *     - Circumcircle / Circumcenter
 *     - Incircle / Incenter
 *     - Triangle-based constructions
 *
 * 14. MINIMUM ENCLOSING CIRCLE
 *     - Smallest circle containing all points
 *     - Welzl's Algorithm
 *     - Expected O(N)
 *
 * 15. POLYGON CUTTING / CLIPPING
 *     - Cut polygon by a line
 *     - Keep one side of a half-plane
 *     - Area after cuts
 *     - Construct feasible regions
 *
 * 16. HALF-PLANE INTERSECTION
 *     - Intersection of inequalities
 *     - Feasible region
 *     - Convex region / kernel
 *     - O(N log N)
 *
 * 17. MINKOWSKI SUM
 *     - Sum of two convex polygons
 *     - Collision detection
 *     - Configuration space
 *     - Convex polygon combination
 *     - O(N + M)
 *
 * 18. CLOSEST PAIR OF POINTS
 *     - Minimum distance between any two points
 *     - Nearest pair
 *     - Sweep Line
 *     - O(N log N)
 *
 * 19. LATTICE / INTEGER GEOMETRY
 *     - Integer-coordinate polygons
 *     - Boundary lattice points
 *     - Interior lattice points
 *     - Pick's Theorem
 *
 * =============================================================================
 *                         QUICK PROBLEM → TOOL MAP
 * =============================================================================
 *
 *  Distance between points       → length()
 *  Perpendicular vectors         → dot()
 *  Left / Right turn             → cross() / orient()
 *  Angle                         → angle() / orientedAngle()
 *  Sort by angle                 → polarComp()
 *
 *  Point → Line                  → line.dist()
 *  Projection                    → line.proj()
 *  Reflection                    → line.refl()
 *  Line × Line                   → inter()
 *  Angle Bisector                → bisector()
 *
 *  Point on Segment              → onSegment()
 *  Segment × Segment             → segInter()
 *  Point → Segment               → distToSegment()
 *
 *  Polygon Area                  → polygonArea()
 *  Point in Polygon              → inPolygon()
 *  Point in Convex Polygon       → inConvexPolygon()
 *  Is Convex?                    → isConvex()
 *  Convex Hull                   → convexHull()
 *  Farthest Pair / Diameter      → convexHullDiameterSq()
 *  Polygon Cut                   → cutPolygon()
 *
 *  Line × Circle                 → interLineCircle()
 *  Circle × Circle               → interCircleCircle()
 *  Point → Circle Tangents       → tangents()
 *  Circle × Circle Tangents      → commonTangents()
 *  Circumcircle                  → circumcircle()
 *  Incircle                      → incircle()
 *  Smallest Enclosing Circle     → minEnclosingCircle()
 *
 *  Half-Plane Intersection       → halfplaneIntersection()
 *  Minkowski Sum                 → minkowskiSum()
 *  Closest Pair                  → closestPair()
 *  Lattice Points                → latticePointsCount()
 *
