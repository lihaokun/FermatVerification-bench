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
  axiomatic ArrayBounds
  {
    predicate
    LowerBound{L}(value_type* a, integer m, integer n, value_type v) =
      \forall integer i; m <= i < n  ==>  v <= a[i];
    predicate
    LowerBound{L}(value_type* a, integer n, value_type v) =
      LowerBound{L}(a, 0, n, v);
    predicate
    StrictLowerBound{L}(value_type* a, integer m, integer n, value_type v) =
      \forall integer i; m <= i < n  ==>  v < a[i];
    predicate
    StrictLowerBound{L}(value_type* a, integer n, value_type v) =
      StrictLowerBound{L}(a, 0, n, v);
    predicate
    UpperBound{L}(value_type* a, integer m, integer n, value_type v) =
      \forall integer i; m <= i < n  ==>  a[i] <= v;
    predicate
    UpperBound{L}(value_type* a, integer n, value_type v) =
      UpperBound{L}(a, 0, n, v);
    predicate
    StrictUpperBound{L}(value_type* a, integer m, integer n, value_type v) =
      \forall integer i; m <= i < n  ==>  a[i] < v;
    predicate
    StrictUpperBound{L}(value_type* a, integer n, value_type v) =
      StrictUpperBound{L}(a, 0, n, v);
  }
*/
/*@
  axiomatic ArrayExtrema
  {
    predicate
    MaxElement{L}(value_type* a, integer n, integer max) =
      0 <= max < n && UpperBound(a, n, a[max]);
    predicate
    MinElement{L}(value_type* a, integer n, integer min) =
      0 <= min < n && LowerBound(a, n, a[min]);
  }
*/
/*@
  requires valid:   \valid_read(a + (0..n-1));
  assigns           \nothing;
  ensures  result:  0 <= \result <= n;
  behavior empty:
    assumes         n == 0;
    assigns         \nothing;
    ensures result: \result == 0;
  behavior not_empty:
    assumes         0 < n;
    assigns         \nothing;
    ensures result: 0 <= \result < n;
    ensures min:    MinElement(a, n, \result);
    ensures first:  StrictLowerBound(a, \result, a[\result]);
  complete behaviors;
  disjoint behaviors;
*/
size_type
min_element(const value_type* a, size_type n)
{
  if (0u < n) {
    size_type min = 0u;
    /*@
      loop invariant bound:  0 <= i   <= n;
      loop invariant min:    0 <= min <  n;
      loop invariant lower:  LowerBound(a, i, a[min]);
      loop invariant first:  StrictLowerBound(a, min, a[min]);
      loop assigns min, i;
      loop variant n-i;
    */
    for (size_type i = 0u; i < n; i++) {
      if (a[i] < a[min]) {
        min = i;
      }
    }
    //@ assert MinElement(a, n, min);
    return min;
  }
  return n;
}