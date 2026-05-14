/*@ requires val >= -2147483647;
    assigns \nothing;
    ensures \old(val) < 0 ==> \result == -\old(val);
    ensures \old(val) >= 0 ==> \result == \old(val);
    ensures \result >= 0;
*/
extern int abs(int val);

int distance(int x, int y)
{
  int tmp;
  /*@ assert rte: signed_overflow: -2147483648 ≤ x - y; */
  /*@ assert rte: signed_overflow: x - y ≤ 2147483647; */
  tmp = abs(x - y);
  return tmp;
}


