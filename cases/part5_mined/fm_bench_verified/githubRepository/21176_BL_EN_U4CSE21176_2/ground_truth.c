#include<limits.h>
/*@
requires INT_MIN<=a<INT_MAX;
requires INT_MIN<=b<INT_MAX;
assigns \nothing;
behavior a_pos:
    assumes a>=0;
    ensures \result == a + 1;
behavior a_neg:
    assumes a < 0;
    ensures \result == a + b;
disjoint behaviors;
complete behaviors;
*/
int foo(int a,int b){
if(a>=0){
a++;
}
else{
a+=b;
}
return a;
}
int main(){
int x=-99;
int y=100;
int r=foo(x,y);
//@assert x==-99 && y==100 && r==1;
}