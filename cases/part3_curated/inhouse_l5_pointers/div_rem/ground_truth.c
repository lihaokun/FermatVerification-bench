void div_rem(unsigned int x, unsigned int y, unsigned int *q, unsigned int *r)
{
  /*@ assert rte: mem_access: \valid(q); */
  /*@ assert rte: division_by_zero: y ≢ 0; */
  *q = x / y;
  /*@ assert rte: mem_access: \valid(r); */
  /*@ assert rte: division_by_zero: y ≢ 0; */
  *r = x % y;
  return;
}


