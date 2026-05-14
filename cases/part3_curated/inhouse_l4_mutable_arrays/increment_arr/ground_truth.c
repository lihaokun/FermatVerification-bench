void increment_arr(int *a, int n, int c)
{
  int i = 0;
  while (i < n) {
    /*@ assert rte: mem_access: \valid(a + i); */
    /*@ assert rte: mem_access: \valid_read(a + i); */
    /*@ assert rte: signed_overflow: -2147483648 ≤ *(a + i) + c; */
    /*@ assert rte: signed_overflow: *(a + i) + c ≤ 2147483647; */
    *(a + i) += c;
    /*@ assert rte: signed_overflow: i + 1 ≤ 2147483647; */
    i ++;
  }
  return;
}


