#include <stdio.h>
/*@
    requires n > 7;
    ensures \result == (n-1)/3;
    assigns \nothing;
*/
int fun(int n) {
    int i = 7;
    int x = 1;
    /*@
    loop invariant i == 4 + 3*x;
	loop invariant i <= n + 3;
	loop assigns x, i;
    */
    while(i <= n) {
        x += 1;
        i += 3;
    }
    return x;
}
int main() {
    int a = fun(10);
    //@ assert a == 3;
}