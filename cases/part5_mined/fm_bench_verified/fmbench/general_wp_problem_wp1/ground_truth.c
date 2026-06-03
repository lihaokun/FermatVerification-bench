#include <limits.h>
/*@
    requires INT_MIN <= x <= INT_MAX;
    requires INT_MIN <= y <= INT_MAX;
    requires -20 <= x + y <= 0;
    ensures \result == x + y + 5;
*/
int function(int x, int y) {
    int res ;
    y += 10 ;
    x -= 5 ;
    res = x + y ;
    //@ assert -15 <= res <= 5;
    return res ;
}
// write a test
void main() {
    int t = function(-5, 5);
    // -5 -5 + 5 + 10 = 5
    //@ assert t == 5;
}