#include<limits.h>
/*@
requires INT_MIN<=a<INT_MAX;
requires INT_MIN<=b<INT_MAX;
ensures \result==a && \result>b || \result==b && \result>a || \result==a && \result==b;
*/
int max(int a,int b){
return a>b?a:b;
}
int main(){
int x=4;
int y=5;
int r=max(x,y);
//@assert x==4 && y==5;
}