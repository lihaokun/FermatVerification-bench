int factorial(int n)
{
  int i = 1;
  int f = 1;
  while (i <= n) {
    /*@ assert rte: signed_overflow: -2147483648 ≤ f * i; */
    /*@ assert rte: signed_overflow: f * i ≤ 2147483647; */
    f *= i;
    /*@ assert rte: signed_overflow: i + 1 ≤ 2147483647; */
    i ++;
  }
  return f;
}


