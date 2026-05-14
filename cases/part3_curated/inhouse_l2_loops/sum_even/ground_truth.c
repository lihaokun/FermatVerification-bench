int sum_even(int n)
{
  int sum = 0;
  int i = 0;
  while (i <= n / 2) {
    /*@ assert rte: signed_overflow: -2147483648 ≤ 2 * i; */
    /*@ assert rte: signed_overflow: 2 * i ≤ 2147483647; */
    /*@ assert rte: signed_overflow: -2147483648 ≤ sum + (int)(2 * i); */
    /*@ assert rte: signed_overflow: sum + (int)(2 * i) ≤ 2147483647; */
    sum += 2 * i;
    /*@ assert rte: signed_overflow: i + 1 ≤ 2147483647; */
    i ++;
  }
  return sum;
}


