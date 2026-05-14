int sum_n(int n)
{
  int s = 0;
  int k = 0;
  while (k <= n) {
    /*@ assert rte: signed_overflow: -2147483648 ≤ s + k; */
    /*@ assert rte: signed_overflow: s + k ≤ 2147483647; */
    s += k;
    /*@ assert rte: signed_overflow: k + 1 ≤ 2147483647; */
    k ++;
  }
  return s;
}


