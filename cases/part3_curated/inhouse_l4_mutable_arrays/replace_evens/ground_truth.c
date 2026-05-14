void replace_evens(int *a, int n)
{
  int i = 0;
  while (i < n) {
    if (i % 2 == 0) 
      /*@ assert rte: mem_access: \valid(a + i); */
      *(a + i) = 0;
    /*@ assert rte: signed_overflow: i + 1 ≤ 2147483647; */
    i ++;
  }
  return;
}


