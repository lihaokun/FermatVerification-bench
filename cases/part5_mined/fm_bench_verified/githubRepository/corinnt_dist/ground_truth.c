#include <limits.h>
/*@ 
    requires INT_MIN < a - b < INT_MAX ||
             INT_MIN < b - a < INT_MAX; 
    ensures positive_value: function_result: \result >= 0;
    ensures correct_sum: (a - b) >= 0 <==> \result == (a - b) &&
                         (a - b) <= 0 <==> \result == (b - a);
    assigns \nothing; 
*/
int distance(int a, int b) {
    if (a < b) return b - a; 
    else return a - b; 
}
int main(){
    int a = -10;
    int b = 10;
    int result = distance(a, b);
    //@ assert result == 20;
    return 0;
}