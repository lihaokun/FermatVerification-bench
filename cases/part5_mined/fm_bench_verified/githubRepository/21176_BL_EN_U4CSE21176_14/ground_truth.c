#include<limits.h>
/*@
requires val>INT_MIN;
assigns \nothing;
behavior pos:
assumes val>0;
ensures \result==val;
behavior neg:
assumes val<0;
ensures \result==-val;
behavior eq:
assumes val==0;
ensures \result==0;
complete behaviors;
disjoint behaviors;
*/
int check(int val){
if(val>0)
return val;
else if(val<0)
return -val;
else
return 0;
}
int main(){
int x=4;
int r=check(x);
//@assert x==4;
}