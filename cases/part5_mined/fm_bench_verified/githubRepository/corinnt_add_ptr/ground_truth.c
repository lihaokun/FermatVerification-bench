#include <limits.h>
/*@
    requires \valid_read(p) && \valid_read(q) && \valid(r); 
    requires INT_MIN < *p < INT_MAX &&
             INT_MIN < *q < INT_MAX &&
             INT_MIN < *p + *q < INT_MAX;
    requires separate_r: \separated(p, r) && \separated(q, r); 
    assigns *r; 
    ensures correct_r: *r == \old(*p) + \old(*q); 
    ensures *r == \result; 
    ensures no_other_effects: *p == \old(*p) && *q == \old(*q); 
*/
int add(int* p, int* q, int* r){
  int result = *p + *q; 
  *r = result;
  return result;
}
int main(){
  int a = 24 ;
  int b = 42 ;
  int res; 
  int x;
  x = add(&a, &b, &res) ;
  //@ assert x == a + b ;
  //@ assert x == 66 ;
  x = add(&a, &a, &res) ;
  //@ assert x == a + a ;
  //@ assert x == 48 ;
}