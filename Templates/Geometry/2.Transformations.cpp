Translation
pt translate(pt v, pt p) {return p+v;}

Scaling
pt scale(pt c,ld factor, pt p) {
  pt ret = p-c;
  ret = factor*ret;
  return c + ret;
}

Rotation
pt rot(pt p, pt c , ld a ) { // point , refrence ponint , alpha
    pt v = p-c;
    pt rotate ={ cos(a ) , sin(a)};
  return c + rotate*v;
  
}
//which if using complex can be simplified to just
pt rot(pt p, double a) {return p * polar(1.0, a);}
//the rotation by 90◦ 
pt perp(pt p) {return {-p.y, p.x};}
