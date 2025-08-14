// point representation
typedef ld T;
	struct pt {
		T x,y;
		//basic operations
		pt operator+(pt p) {return {x+p.x, y+p.y};}
		pt operator-(pt p) {return {x-p.x, y-p.y};}
		pt operator*(T d) {return {x*d, y*d};}
		pt operator/(T d) {return {x/d, y/d};} // only for floating point
	};
  //example of usage
	// int a, b ;
	// cin>>a >> b;
	// pt p1 = {a, b};    warning for diffrent data type not error
or add these definitons
typedef ld T;
typedef complex<T> pt;
#define x real()
#define y imag()
--------------------------------------
// theta with x axis
  cout<<arg(p) --> radian
  // transfer to degree
  arg(p) * (180/ M_PI) --> PI Value
// atan2
atan2(b,a) 
----------------------------------------
// Functions
sq -- > distance from point to (0,0)  // r ^ 2
T sq (pt p){
  return p.x * p.x + p.y * p.y;
}

cout<<abs(p)---> magnitude (r) not removing the negative value

int sgn ( T val){  
  if(val < -EPS)return -1;
  else if (val > EPS ) return 1;
  else return 0;
}








