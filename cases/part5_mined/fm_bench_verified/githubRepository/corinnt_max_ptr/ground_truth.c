#include <stddef.h>
#include <limits.h>
/*@ requires \valid(a) && \valid(b); 
    assigns *a, *b; 
    ensures a_in_nums: *a \in { \old(*a), \old(*b) } ; 
    ensures b_in_nums: *b \in { \old(*a), \old(*b) } ; 
    ensures a_is_max: *a >= \old(*a) && *a >= \old(*b); 
    ensures b_is_min: *b <= \old(*a) && *b <= \old(*b); 
    behavior a_max:
      assumes *a > *b; 
      ensures *a == \old(*a) && *b == \old(*b); 
    behavior b_max: 
      assumes *b > *a;
      ensures *a == \old(*b) && *b == \old(*a); 
    behavior equals: 
      assumes *a == *b; 
      ensures *a == \old(*a) && *b == \old(*b); 
    complete behaviors; 
    disjoint behaviors; 
*/
void max_ptr(int* a, int* b) {
    if(*a < *b) {
      int tmp = *b ;
      *b = *a ;
      *a = tmp; 
    }
}
extern int h ;
int main() {
  h = 42 ;
  int a = 24 ;
  int b = 42 ;
  max_ptr(&a, &b) ;
  //@ assert a == 42 ;
  //@ assert b == 24 ;
}