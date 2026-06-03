/*@
    requires n > 0;
    requires \valid_read(a+(0..n-1));
    ensures \forall integer k; 0 <= k < n ==> a[k] == \old(a[k])*2;
*/
void arrayDouble(int *a, unsigned int n) {
    int p = 0;
    /*@
        loop invariant 0 <= p <= n;
        loop invariant \forall integer k; 0 <= k < p ==> a[k] == \at(a[k], Pre) * 2;
        loop invariant \forall integer k; p <= k < n ==> a[k] == \at(a[k], Pre);
        loop assigns p, a[0..n-1];
        loop variant n - p;
    */
    while (p < n) {
        a[p] = a[p] * 2;
        p = p + 1;
    } 
}
int main() {
    int arr[] = {1,3,5,7,8};
    arrayDouble(arr, 5);
    //@ assert arr[0] == 2;
    //@ assert arr[1] == 6;
    //@ assert arr[2] == 10;    
    //@ assert arr[3] == 14;
    //@ assert arr[4] == 16;
    return 0;
}