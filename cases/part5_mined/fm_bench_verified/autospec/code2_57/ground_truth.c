/*@
requires n > 0;
*/
void foo(int n) {
  int c = 0;
  /*@
  loop invariant n-c;
  loop invariant n > -1 ==> c != n;
  loop invariant c >= 0 && c <= n+1;
  loop invariant 0 <= c <= n;
  loop invariant c != n;
  loop assigns n,c;
  */
  while (unknown()) {
    if (unknown()) {
      if (c > n) {
        c = c + 1;
      }
    } else {
      if (c == n) {
        c = 1;
      }
    }
  }
  if (n > -1) {
    //@ assert c != n;
  }
}