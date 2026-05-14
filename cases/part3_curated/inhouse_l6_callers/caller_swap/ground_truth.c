/*@ requires \separated(a, b);
    requires \valid(a);
    requires \valid(b);
    assigns *a, *b;
    ensures *a == \old(*b);
    ensures *b == \old(*a);
*/
extern void swap(int *a, int *b);

void sort2(int *a, int *b)
{
  /*@ assert rte: mem_access: \valid_read(a); */
  /*@ assert rte: mem_access: \valid_read(b); */
  if (*a > *b) swap(a,b);
  return;
}


