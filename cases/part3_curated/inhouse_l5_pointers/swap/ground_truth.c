void swap(int *a, int *b)
{
  /*@ assert rte: mem_access: \valid_read(a); */
  int tmp = *a;
  /*@ assert rte: mem_access: \valid(a); */
  /*@ assert rte: mem_access: \valid_read(b); */
  *a = *b;
  /*@ assert rte: mem_access: \valid(b); */
  *b = tmp;
  return;
}


