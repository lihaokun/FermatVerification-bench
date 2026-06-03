#include<limits.h>
/*@
requires INT_MIN<=val<INT_MAX;
ensures (val>=0 ==> \result == val) && (val<0 ==> \result == -val);
*/
int abs(int val){
if(val<0)
return -val;
else
return val;
}
int main(){
int x=4;
int r=abs(x);
//@assert x==4;
}