/*@
    requires x ==1 && y==2;
    ensures *r < y;
    ensures \result == 0 <==> x < y;
    ensures x == \result*y + *r;
    ensures \result ==0 ==> x == *r;
*/
int fun(int x, int y , int *r) {
    *r = x;
    int d = 0;
    /*@
        loop invariant d >= 0;
        loop invariant *r <= x;
        loop invariant *r < y ==> d == 0;
        loop invariant *r == x - y*d;
        loop assigns *r, d;
    */
    while (*r >= y) {
        *r = *r - y;
        d = d + 1;
    }
    return d;
}
int main() {
    int a = 3;
    int num = fun(1, 2, &a);
    //@ assert a == 1;
    //@ assert num == 0;
    return 0;
}