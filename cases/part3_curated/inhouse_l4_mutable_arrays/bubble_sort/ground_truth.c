void bubble_sort(int *a, int n)
{
  int i;
  int j;
  int temp;
  if (n <= 0) goto return_label;
  /*@ assert rte: signed_overflow: -2147483648 ≤ n - 1; */
  i = n - 1;
  while (i > 0) {
    j = 0;
    while (j < i) {
      /*@ assert rte: mem_access: \valid_read(a + j); */
      /*@ assert rte: signed_overflow: j + 1 ≤ 2147483647; */
      /*@ assert rte: mem_access: \valid_read(a + (int)(j + 1)); */
      if (*(a + j) > *(a + (j + 1))) {
        /*@ assert rte: mem_access: \valid_read(a + j); */
        temp = *(a + j);
        /*@ assert rte: mem_access: \valid(a + j); */
        /*@ assert rte: signed_overflow: j + 1 ≤ 2147483647; */
        /*@ assert rte: mem_access: \valid_read(a + (int)(j + 1)); */
        *(a + j) = *(a + (j + 1));
        /*@ assert rte: mem_access: \valid(a + (int)(j + 1)); */
        /*@ assert rte: signed_overflow: j + 1 ≤ 2147483647; */
        *(a + (j + 1)) = temp;
      }
      /*@ assert rte: signed_overflow: j + 1 ≤ 2147483647; */
      j ++;
    }
    /*@ assert rte: signed_overflow: -2147483648 ≤ i - 1; */
    i --;
  }
  return_label: return;
}


