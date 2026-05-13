/* ===== include: adjacent_difference.c ===== */

/* ===== include: adjacent_difference.h ===== */

#ifndef ADJACENT_DIFFERENCE_H_INCLUDED
#define ADJACENT_DIFFERENCE_H_INCLUDED

/* ===== include: AdjacentDifference.acsl ===== */

#ifndef ADJACENTDIFFERENCE_ACSL_INCLUDED
#define ADJACENTDIFFERENCE_ACSL_INCLUDED

/* could not resolve: #include "limits.h" */
/* ===== include: Difference.acsl ===== */

#ifndef DIFFERENCE_ACSL_INCLUDED
#define DIFFERENCE_ACSL_INCLUDED

/* ===== include: Unchanged.acsl ===== */

#ifndef UNCHANGED_ACSL_INCLUDED
#define UNCHANGED_ACSL_INCLUDED

/* ===== include: typedefs.h ===== */

#ifndef TYPEDEFS_H_INCLUDED
#define TYPEDEFS_H_INCLUDED

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

#endif /* TYPEDEFS_H_INCLUDED */



/*@
  predicate Unchanged{K,L}(value_type* a, integer m, integer n) =
    \forall integer i; m <= i < n ==>  \at(a[i],K) == \at(a[i],L);

  predicate Unchanged{K,L}(value_type* a, integer n) = Unchanged{K,L}(a, 0, n);
*/

#endif /* UNCHANGED_ACSL_INCLUDED */



/*@
  logic integer
  Difference{L}(value_type* a, integer n) =
    n <= 0 ? a[0] : a[n] - a[n-1];

  lemma Difference_Zero{L}:
    \forall value_type *a; Difference(a, 0) == a[0];

  lemma Difference_Next{L}:
    \forall value_type *a, integer n;
      0 < n  ==>  Difference(a, n) == a[n] - a[n-1];

  lemma Difference_Unchanged{K,L}:
    \forall value_type *a, integer n;
      0 <= n  ==>  Unchanged{K,L}(a, n+1)  ==>
      Difference{K}(a, n) == Difference{L}(a, n);
*/

#endif /* DIFFERENCE_ACSL_INCLUDED */



/*@
    predicate AdjacentDifference{L}(value_type* a, integer n, value_type* b) =
      \forall integer i; 0 <= i < n  ==>  b[i] == Difference(a, i);

    predicate AdjacentDifferenceBounds(value_type* a, integer n) =
      \forall integer i; 1 <= i < n  ==>
        VALUE_TYPE_MIN <= Difference(a, i) <= VALUE_TYPE_MAX;

    lemma AdjacentDifference_Step{K,L}:
      \forall value_type *a, *b, integer n;
        AdjacentDifference{K}(a, n, b)      ==>
        Unchanged{K,L}(b, n)                ==>
        Unchanged{K,L}(a, n+1)              ==>
        \at(b[n],L) == Difference{L}(a, n)  ==>
        AdjacentDifference{L}(a, n+1, b);

    lemma AdjacentDifference_Section{K}:
      \forall value_type *a, *b, integer m, n;
        0 <= m <= n                     ==>
        AdjacentDifference{K}(a, n, b)  ==>
        AdjacentDifference{K}(a, m, b);
*/

#endif /* ADJACENTDIFFERENCE_ACSL_INCLUDED */



/*@
   requires   valid:       \valid_read(a + (0..n-1));
   requires   valid:       \valid(b + (0..n-1));
   requires   sep:         \separated(a + (0..n-1), b + (0..n-1));
   requires   bounds:      AdjacentDifferenceBounds(a, n);

   terminates              \true;
   exits                   \false;
   assigns                 b[0..n-1];

   ensures    result:      \result == n;
   ensures    difference:  AdjacentDifference(a, n, b);
   ensures    unchanged:   Unchanged{Old,Here}(a, n);
*/
size_type adjacent_difference(const value_type* a, size_type n, value_type* b);

#endif /* ADJACENT_DIFFERENCE_H_INCLUDED */



size_type adjacent_difference(const value_type* a, size_type n, value_type* b)
{
  if (0u < n) {
    b[0u] = a[0u];

    /*@
       loop invariant index:      1 <= i <= n;
       loop invariant unchanged:  Unchanged{Pre,Here}(a, n);
       loop invariant difference: AdjacentDifference(a, i, b);
       loop assigns i, b[1..n-1];
       loop variant n - i;
    */
    for (size_type i = 1u; i < n; ++i) {
      //@ assert bound: VALUE_TYPE_MIN <= Difference(a, i) <= VALUE_TYPE_MAX;
      b[i] = a[i] - a[i - 1u];
      //@ assert difference: AdjacentDifference(a, i+1, b);
    }
  }

  return n;
}

