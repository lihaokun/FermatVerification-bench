int triangle(int a, int b, int c)
{
  int __retres;
  /*@ assert rte: signed_overflow: -2147483648 ≤ a + b; */
  /*@ assert rte: signed_overflow: a + b ≤ 2147483647; */
  /*@ assert rte: signed_overflow: -2147483648 ≤ (int)(a + b) + c; */
  /*@ assert rte: signed_overflow: (int)(a + b) + c ≤ 2147483647; */
  if ((a + b) + c == 180) {
    if (a > 0) {
      if (b > 0) {
        if (c > 0) {
          __retres = 1;
          goto return_label;
        }
        else goto _LAND_1;
      }
      else goto _LAND_1;
    }
    else goto _LAND_1;
  }
  else {
    _LAND_1: {
               __retres = 0;
               goto return_label;
             }
  }
  return_label: return __retres;
}


