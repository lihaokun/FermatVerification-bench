int incr_a_by_b(int *a, int const *b)
{
  int __retres;
  /*@ assert rte: mem_access: \valid(a); */
  /*@ assert rte: mem_access: \valid_read(a); */
  /*@ assert rte: mem_access: \valid_read(b); */
  /*@ assert rte: signed_overflow: -2147483648 ≤ *a + *b; */
  /*@ assert rte: signed_overflow: *a + *b ≤ 2147483647; */
  *a += *b;
  /*@ assert rte: mem_access: \valid_read(a); */
  __retres = *a;
  return __retres;
}


