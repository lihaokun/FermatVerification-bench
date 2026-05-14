int binary_search(int *a, int x, int n)
{
  int __retres;
  int p;
  int low = -1;
  int high = n;
  while (1) {
    /*@ assert rte: signed_overflow: low + 1 ≤ 2147483647; */
    if (! (low + 1 < high)) break;
    /*@ assert rte: signed_overflow: -2147483648 ≤ low + high; */
    /*@ assert rte: signed_overflow: low + high ≤ 2147483647; */
    p = (low + high) / 2;
    /*@ assert rte: mem_access: \valid_read(a + p); */
    if (*(a + p) == x) {
      __retres = p;
      goto return_label;
    }
    else 
      /*@ assert rte: mem_access: \valid_read(a + p); */
      if (*(a + p) < x) low = p; else high = p;
  }
  __retres = -1;
  return_label: return __retres;
}


