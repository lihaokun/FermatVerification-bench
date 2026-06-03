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
  axiomatic Reverse
  {
    predicate
    Reverse{K,L}(value_type* a, integer n, value_type* b) =
      \forall integer i; 0 <= i < n  ==>  \at(a[i],K) == \at(b[n-1-i], L);
    predicate
    Reverse{K,L}(value_type* a, integer m, integer n,
                 value_type* b, integer p) = Reverse{K,L}(a+m, n-m, b+p);
    predicate
    Reverse{K,L}(value_type* a, integer m, integer n, value_type* b) =
      Reverse{K,L}(a, m, n, b, m);
    predicate
    Reverse{K,L}(value_type* a, integer m, integer n, integer p) =
      Reverse{K,L}(a, m, n, a, p);
    predicate
    Reverse{K,L}(value_type* a, integer m, integer n) =
      Reverse{K,L}(a, m, n, m);
    predicate
    Reverse{K,L}(value_type* a, integer n) = Reverse{K,L}(a, 0, n);
  }
*/
/*@
  axiomatic Unchanged
  {
    predicate
    Unchanged{K,L}(value_type* a, integer m, integer n) =
      \forall integer i; m <= i < n ==>  \at(a[i],K) == \at(a[i],L);
    predicate
    Unchanged{K,L}(value_type* a, integer n) = Unchanged{K,L}(a, 0, n);
  }
*/
/*@
  requires valid:      \valid_read(a + (0..n-1));
  requires valid:      \valid(b + (0..n-1));
  requires sep:        \separated(a + (0..n-1), b + (0..n-1));
  assigns              b[0..(n-1)];
  ensures reverse:     Reverse{Old,Here}(a, n, b);
  ensures unchanged:   Unchanged{Old,Here}(a, n);
*/
void
reverse_copy(const value_type* a, size_type n, value_type* b)
{
  /*@
    loop invariant bound:   0 <= i <= n;
    loop invariant reverse: Reverse{Here,Pre}(b, 0, i, a, n-i);
    loop assigns i, b[0..n-1];
    loop variant n-i;
  */
  for (size_type i = 0u; i < n; ++i) {
    b[i] = a[n - 1u - i];
  }
  //@ assert Reverse{Pre,Here}(a, n, b);
}