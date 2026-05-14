/*@ requires x > -2147483648;
    ensures \result >= 0;
    ensures x >= 0 ==> \result == x;
    ensures x < 0 ==> \result == -x;
    assigns \nothing;
*/
int abs_val(int x) {
    if (x < 0) {
        return -x;
    }
    return x;
}

/*@ requires n >= 0;
    ensures \result >= 0;
    assigns \nothing;
*/
int double_abs(int n) {
    int r = abs_val(n);
    return r + r;
}
