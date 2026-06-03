/*@
  requires \valid(s + (0 .. n - 1)) && n >= 1;
  assigns s[0 .. n - 1];
  ensures \forall integer k; 0 <= k < n ==> s[k] == 0;
*/
void firstSubset(int s[], int n) {
  /*@
    loop invariant 0 <= k <= n;
    loop invariant \forall integer i; 0 <= i < k ==> s[i] == 0;
    loop assigns k, s[0 .. n - 1];
    loop variant n - k;
  */
 for (int k = 0; k < n; k++) {
  s[k] = 0;
 }
}
int main(){
  int s[3];
  firstSubset(s, 3);
  //@ assert s[0] == 0 && s[1] == 0 && s[2] == 0;
}