int add_ptr(int *p, int *q)
{
  int __retres;
  /*@ assert rte: mem_access: \valid_read(p); */
  /*@ assert rte: mem_access: \valid_read(q); */
  /*@ assert rte: signed_overflow: -2147483648 ≤ *p + *q; */
  /*@ assert rte: signed_overflow: *p + *q ≤ 2147483647; */
  __retres = *p + *q;
  return __retres;
}


