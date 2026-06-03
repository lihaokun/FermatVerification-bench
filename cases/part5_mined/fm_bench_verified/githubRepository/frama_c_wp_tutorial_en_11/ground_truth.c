#include<limits.h>
/*@
    logic integer ax_b(integer a, integer x, integer b) = a * x + b;
*/
/*@
    requires INT_MIN <= 3*x;
    requires INT_MIN <= ax_b(3, x, 4) <= INT_MAX;
    assigns \nothing;
    ensures \result == ax_b(3, x, 4);
*/
int restricted(int x){
    return 3*x + 4;
}
int main(){
    int x = 10;
    int result = restricted(x);
    //@ assert result == 34;
    return 0;
}