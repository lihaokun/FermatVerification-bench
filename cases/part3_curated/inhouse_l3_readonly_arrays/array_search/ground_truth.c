int array_search(int *a, int x, int n)
{
  int __retres;
  int p = 0;
  while (p < n) {
    /*@ assert rte: mem_access: \valid_read(a + p); */
    if (*(a + p) == x) {
      __retres = 1;
      goto return_label;
    }
    /*@ assert rte: signed_overflow: p + 1 ≤ 2147483647; */
    p ++;
  }
  __retres = 0;
  return_label: return __retres;
}


