int diff(int x, int y)
{
  int __retres;
  /*@ assert rte: signed_overflow: -2147483648 ≤ x - y; */
  /*@ assert rte: signed_overflow: x - y ≤ 2147483647; */
  __retres = x - y;
  return __retres;
}


