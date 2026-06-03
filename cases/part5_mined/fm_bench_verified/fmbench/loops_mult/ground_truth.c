#include <stdio.h>
/*@
    requires a >= 0 && b >= 0;
    requires a == 2 && b == 5;
    ensures \result == \old(a+1) * \old(b);
    assigns \nothing;
*/
int mul(int a, int b) {
    int x = a, y = b, prod = 0;
    /*@ 
        loop invariant prod == (a-x)*y;
        loop invariant x == a - prod/y;
        loop invariant -1 <= x <= a;
        loop assigns prod, x;
        loop variant x;
    */
    while(x >= 0) {
        prod = prod + y;
        x--;
    }
    return prod;
}
int main() {
    int pdt = mul(2, 5);
    //@ assert pdt == 15;
}