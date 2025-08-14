use EPS for Comparing
double --> 17 dig , ld --->20 dig
// overall digits including the digits before precision

const ld EPS = 1e-9;

ld a = 0 , b =0.00000000001;
if( a == b ) {
  مش هيحصل
}
fabs--> float abs
  
if( fabs(a - b ) < EPS) {
  تمام
}

