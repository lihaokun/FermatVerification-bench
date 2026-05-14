int area(int base, int height)
{
  /*@ assert rte: signed_overflow: -2147483648 ≤ base * height; */
  /*@ assert rte: signed_overflow: base * height ≤ 2147483647; */
  int res = (base * height) / 2;
  return res;
}


