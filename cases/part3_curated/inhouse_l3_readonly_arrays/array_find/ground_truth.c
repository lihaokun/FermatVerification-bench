int array_find(int *a, int n, int x)
{
  int __retres;
  int i = 0;
  i = 0;
  while (i < n) {
    /*@ assert rte: mem_access: \valid_read(a + i); */
    if (*(a + i) == x) {
      __retres = i;
      goto return_label;
    }
    /*@ assert rte: signed_overflow: i + 1 ≤ 2147483647; */
    i ++;
  }
  __retres = -1;
  return_label: return __retres;
}


