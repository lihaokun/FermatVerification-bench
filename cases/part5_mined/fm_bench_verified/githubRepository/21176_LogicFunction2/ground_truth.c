#include<limits.h>
/*@
logic integer ax_b(integer a,integer x,integer b) = a*x+b;
*/
/*@
assigns \nothing;
ensures \result==ax_b(a,x,4);
*/
int func(int a,int x){
return a*x+4;
}
/*@
requires INT_MIN<=a*x<INT_MAX;
requires INT_MIN<=a*y<INT_MAX;
requires a>0;
requires INT_MIN<=ax_b(a,x,4)<=INT_MAX;
requires INT_MIN<=ax_b(a,y,4)<=INT_MAX;
assigns \nothing;
*/
void check(int a,int x,int y){
int fmin,fmax;
if(x < y){}
else{
    int temp = x;
    x = y;
    y = temp;
}
fmin=func(a,x);
fmax=func(a,y);
//@assert fmin== ax_b(a, (x < y ? x : y), 4);
}
int main(){
int w=3;
int r=4;
int s=7;
int t=func(w,r);
check(w,r,s);
}