/*@ 
   assigns \nothing;
    ensures \result == 0 || \result == 1 || \result == -1;
    behavior positive:
       assumes x > 0;
       ensures \result == 1;
    behavior zero:
       assumes x == 0;
       ensures \result == 0;
    behavior negative:
       assumes x < 0;
       ensures \result == -1;
    complete behaviors;
    disjoint behaviors;
 */
int spec_sign(int x)
{
   return (x > 0) - (x < 0);
}
#ifdef OUT_OF_TASK
#include <stdio.h>
int main(void)
{
   int res = spec_sign(-10);
   //@ assert res == -1;
   return 0;
}
#endif