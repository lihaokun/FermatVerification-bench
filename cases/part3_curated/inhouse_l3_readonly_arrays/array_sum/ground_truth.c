int array_sum(int *a, int n)
{
  int p = 0;
  int sum = 0;
  while (p < n) {
    /*@ assert rte: mem_access: \valid_read(a + p); */
    /*@ assert rte: signed_overflow: -2147483648 ≤ sum + *(a + p); */
    /*@ assert rte: signed_overflow: sum + *(a + p) ≤ 2147483647; */
    sum += *(a + p);
    /*@ assert rte: signed_overflow: p + 1 ≤ 2147483647; */
    p ++;
  }
  return sum;
}


