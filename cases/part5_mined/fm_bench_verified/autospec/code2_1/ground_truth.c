void main() {
  int x;
  int y;
  x = 1;
  y = 0;
  /*@
  loop invariant y <= x;
  loop invariant 0 <= y <= 100000;
  loop invariant 1 <= x;
  loop assigns y,x;
  */
  while (y < 100000) {
    x  = x + y;
    y  = y + 1;
  }
  //@ assert  (x >= y) ;
}