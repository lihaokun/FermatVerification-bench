#include<math.h>
/*@
	requires \is_finite(x);
	requires x >= -0.;
	ensures \result >= -0.;
	ensures \is_finite(\result);
	ensures \result == sqrt(x);
	assigns \result;
*/
double Sqrt(double x){	
	return sqrt(x);
}
int main(){
	double x = 9;
	double y;
	y = Sqrt(x);
	//@assert y == sqrt(x); 
	return 0;
}