/*@
requires n > 0;
*/
void foo(int n) {
  int c = 0;
  /*@
  loop invariant 0 <= c <= n;
  loop invariant c <= n || 0 < n;
  loop invariant 0 < n;
  loop invariant (c == n || c != n);
  loop assigns n,c;
  */
  while (unknown()) {
    if (unknown()) {
      if (c != n) {
        c = c + 1;
      }
    } else {
      if (c == n) {
        c = 1;
      }
    }
  }
  if (c != n) {
    //@ assert c <= n;
  }
}