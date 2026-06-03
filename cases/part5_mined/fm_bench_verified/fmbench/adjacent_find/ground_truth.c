#include <limits.h>
#ifndef __cplusplus
typedef int bool;
#define false		((bool)0)
#define true		((bool)1)
#endif
typedef int value_type;
#define VALUE_TYPE_MAX  INT_MAX
#define VALUE_TYPE_MIN  INT_MIN
typedef unsigned int size_type;
#define SIZE_TYPE_MAX  UINT_MAX
/*@
  axiomatic HasEqualNeighbors
  {
    predicate
    HasEqualNeighbors{L}(value_type* a, integer n) =
      \exists integer i; 0 <= i < n-1 && a[i] == a[i+1];
  }
*/
/*@
  requires valid:       \valid_read(a + (0..n-1));
  assigns               \nothing;
  ensures result:       0 <= \result <= n;
  behavior some:
    assumes             HasEqualNeighbors(a, n);
    assigns             \nothing;
    ensures  result:    0 <= \result < n-1;
    ensures  adjacent:  a[\result] == a[\result+1];
    ensures  first:     !HasEqualNeighbors(a, \result);
  behavior none:
    assumes             !HasEqualNeighbors(a, n);
    assigns             \nothing;
    ensures  result:    \result == n;
  complete behaviors;
  disjoint behaviors;
*/
size_type
adjacent_find(const value_type* a, size_type n)
{
  if (1u < n) {
    /*@
      loop invariant bound:  0 <= i < n;
      loop invariant none:   !HasEqualNeighbors(a, i+1);
      loop assigns i;
      loop variant n-i;
    */
    for (size_type i = 0u; i + 1u < n; ++i) {
      if (a[i] == a[i + 1u]) {
        //@ assert HasEqualNeighbors(a, i+2);
        return  i;
      }
    }
  }
  //@ assert !HasEqualNeighbors(a, n);
  return n;
}