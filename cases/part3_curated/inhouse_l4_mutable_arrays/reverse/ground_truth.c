void reverse(int *a, int n)
{
  int i = 0;
  /*@ assert rte: signed_overflow: -2147483648 ≤ n - 1; */
  int j = n - 1;
  while (i < n / 2) {
    /*@ assert rte: mem_access: \valid_read(a + i); */
    int temp = *(a + i);
    /*@ assert rte: mem_access: \valid(a + i); */
    /*@ assert rte: mem_access: \valid_read(a + j); */
    *(a + i) = *(a + j);
    /*@ assert rte: mem_access: \valid(a + j); */
    *(a + j) = temp;
    /*@ assert rte: signed_overflow: i + 1 ≤ 2147483647; */
    i ++;
    /*@ assert rte: signed_overflow: -2147483648 ≤ j - 1; */
    j --;
  }
  return;
}


