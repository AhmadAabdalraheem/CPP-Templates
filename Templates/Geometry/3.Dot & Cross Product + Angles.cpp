T dot(pt v, pt w){
    return v.x * w.x + v.y * w.y;
}
 
T cross(pt v, pt w){
    return v.x * w.y - v.y * w.x;
}

T angle(pt v, pt w){ // الزاوية الصغيرة مش المنعكسة
    return acos(clamp(dot(v, w) / abs(v) / abs(w), (T)-1.0, (T)1.0));
}
T orient(pt a, pt b, pt c){
    // كانك خارج ب خط من نقطة 1 لنقطة 2 و بتشوف نقطة 3 يمينك ولا شمالك
    // لو شمالك موجب لو يمينك سالب
    return cross(b - a, c - a);
}

bool isPerpendicular(pt v, pt w){
    return fabs(dot(v, w)) < EPS;
}
pt prep(pt p){ //(a,b) ---> (-b,a ) عمودي عليها وعلي شمالها
    return {-p.y, p.x};
}

T orientedAngle(pt a, pt b, pt c){ 
      // الزاوية المنعكسة 
    //  < BAC IN COUNTER CLOCK WISE
    ld ampli = angle(b - a, c - a);
    if(orient(a, b, c) > 0) return ampli;
    else return 2 * M_PI - ampli;
}
T angleTravelled(pt a, pt b, pt c){
    ld ampli = angle(b - a, c - a);
    if(orient(a, b, c) > 0) return ampli;
    else return -ampli;
}
