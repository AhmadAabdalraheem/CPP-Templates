T dot(pt v, pt w){
    return v.x * w.x + v.y * w.y;
}
 
T cross(pt v, pt w){
    return v.x * w.y - v.y * w.x;
}
