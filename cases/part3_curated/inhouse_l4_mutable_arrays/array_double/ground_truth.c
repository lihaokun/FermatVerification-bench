void array_double(int *a, unsigned int n)
{
  int p = 0;
  while ((unsigned int)p < n) {
    /*@ assert rte: mem_access: \valid(a + p); */
    /*@ assert rte: mem_access: \valid_read(a + p); */
    /*@ assert rte: signed_overflow: -2147483648 ≤ *(a + p) * 2; */
    /*@ assert rte: signed_overflow: *(a + p) * 2 ≤ 2147483647; */
    *(a + p) *= 2;
    /*@ assert rte: signed_overflow: p + 1 ≤ 2147483647; */
    p ++;
  }
  return;
}


