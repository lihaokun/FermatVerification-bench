int array_max(int *a, int n)
{
  int i = 1;
  /*@ assert rte: mem_access: \valid_read(a + 0); */
  int max = *(a + 0);
  while (i < n) {
    /*@ assert rte: mem_access: \valid_read(a + i); */
    if (max < *(a + i)) 
      /*@ assert rte: mem_access: \valid_read(a + i); */
      max = *(a + i);
    /*@ assert rte: signed_overflow: i + 1 ≤ 2147483647; */
    i ++;
  }
  return max;
}


