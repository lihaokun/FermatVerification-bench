/* ===== include: adjacent_difference_inv.c ===== */

/* ===== include: partial_sum.h ===== */

#ifndef PARTIAL_SUM_H_INCLUDED
#define PARTIAL_SUM_H_INCLUDED

/* ===== include: PartialSum.acsl ===== */
#ifndef PARTIALSUM_ACSL_INCLUDED
#define PARTIALSUM_ACSL_INCLUDED

/* ===== include: AccumulateDefault.acsl ===== */

#ifndef ACCUMULATEDEFAULT_ACSL_INCLUDED
#define ACCUMULATEDEFAULT_ACSL_INCLUDED

/* ===== include: Accumulate.acsl ===== */

#ifndef ACCUMULATE_ACSL_INCLUDED
#define ACCUMULATE_ACSL_INCLUDED

/* ===== include: UnchangedLemmas.acsl ===== */

#ifndef UNCHANGEDLEMMAS_ACSL_INCLUDED
#define UNCHANGEDLEMMAS_ACSL_INCLUDED

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
  lemma Unchanged_Shrink{K,L}:
    \forall value_type *a, integer m, n, p, q;
       m <= p <= q <= n         ==>
       Unchanged{K,L}(a, m, n)  ==>
       Unchanged{K,L}(a, p, q);

  lemma Unchanged_Extend{K,L}:
    \forall value_type *a, integer n;
      Unchanged{K,L}(a, n)        ==>
      \at(a[n],K) == \at(a[n],L)  ==>
      Unchanged{K,L}(a, n+1);

  lemma Unchanged_Shift{K,L}:
    \forall value_type *a, integer p, q, r;
      Unchanged{K,L}(a+p, q, r)  ==>  Unchanged{K,L}(a, p+q, p+r);

  lemma Unchanged_Symmetric{K,L}:
    \forall value_type *a, integer m, n;
      Unchanged{K,L}(a, m, n)  ==>
      Unchanged{L,K}(a, m, n);

  lemma Unchanged_Transitive{K,L,M}:
    \forall value_type *a, integer m, n;
      Unchanged{K,L}(a, m, n)  ==>
      Unchanged{L,M}(a, m, n)  ==>
      Unchanged{K,M}(a, m, n);
*/

#endif /* UNCHANGEDLEMMAS_ACSL_INCLUDED */


/* could not resolve: #include "limits.h" */

/*@
    logic integer
    Accumulate{L}(value_type* a, integer n, integer init) =
      n <= 0 ? init : Accumulate(a, n-1, init) + a[n-1];

    predicate AccumulateBounds{L}(value_type* a, integer n, value_type init) =
      \forall integer i; 0 <= i <= n  ==>
        VALUE_TYPE_MIN <= Accumulate(a, i, init) <= VALUE_TYPE_MAX;

    lemma Accumulate_Init:
      \forall value_type *a, init, integer n;
         n <= 0  ==>  Accumulate(a, n, init) == init;

    lemma Accumulate_Next{L}:
      \forall value_type* a, init, integer n;
        0 <= n  ==>
        Accumulate(a, n+1, init) == Accumulate(a, n, init) + a[n];

    lemma Accumulate_Unchanged{K,L}:
      \forall value_type *a, init, integer n;
        Unchanged{K,L}(a, n)  ==>
        Accumulate{K}(a, n, init) == Accumulate{L}(a, n, init);

    lemma Accumulate_Unchanged_Shrink{K,L}:
      \forall value_type *a, init, integer m, n;
        0 <= m <= n            ==>
        Unchanged{K, L}(a, n)  ==>
        Accumulate{K}(a, m, init) == Accumulate{L}(a, m, init);

    lemma AccumulateBounds_Unchanged{K,L}:
      \forall value_type *a, init, integer n;
        Unchanged{K, L}(a, n)           ==>
        AccumulateBounds{K}(a, n, init) ==>
        AccumulateBounds{L}(a, n, init);
*/

#endif /* ACCUMULATE_ACSL_INCLUDED */



/*@
    logic integer
    AccumulateDefault{L}(value_type* a, integer n) =
      Accumulate(a, n, (value_type)(0));

    predicate AccumulateDefaultBounds{L}(value_type* a, integer n) =
      AccumulateBounds(a, n, (value_type)(0));

    lemma AccumulateDefault_Unchanged{K,L}:
      \forall value_type *a, integer n;
        0 <= n                ==>
        Unchanged{K,L}(a, n)  ==>
        AccumulateDefault{K}(a, n) == AccumulateDefault{L}(a, n);

    lemma AccumulateDefault_Zero{L}:
      \forall value_type* a; AccumulateDefault(a, 0) == 0;

    lemma AccumulateDefault_One{L}:
      \forall value_type* a; AccumulateDefault(a, 1) == a[0];

    lemma AccumulateDefault_Next{L}:
      \forall value_type* a, integer n;
        0 <= n  ==>
        AccumulateDefault(a, n+1) == AccumulateDefault(a, n) + a[n];

    lemma AccumulateDefaultBounds_Shrink{L}:
      \forall value_type* a, integer m, n;
        0 <= m <= n                    ==>
        AccumulateDefaultBounds(a, n)  ==>  AccumulateDefaultBounds(a, m);
*/

#endif /* ACCUMULATEDEFAULT_ACSL_INCLUDED */



/*@
  predicate PartialSum{L}(value_type* a, integer n, value_type* b) =
    \forall integer i; 0 <= i < n  ==>  b[i] == AccumulateDefault(a, i+1);

  lemma PartialSum_Section{K}:
    \forall value_type *a, *b, integer m, n;
    0 <= m <= n             ==>
    PartialSum{K}(a, n, b)  ==>  PartialSum{K}(a, m, b);

  lemma PartialSum_Step{L}:
    \forall value_type *a, *b, integer n;
      0 <= n                             ==>
      PartialSum(a, n, b)                ==>
      b[n] == AccumulateDefault(a, n+1)  ==>  PartialSum(a, n+1, b);

  lemma PartialSum_Unchanged{K,L}:
    \forall value_type *a, *b, integer n;
      0 <= n  ==>
      PartialSum{K}(a, n, b)  ==>
      Unchanged{K, L}(a, n)   ==>
      Unchanged{K, L}(b, n)   ==>  PartialSum{L}(a, n, b);

  lemma PartialSum_One{L}:
    \forall value_type *a, *b, integer n;
      b[0] == AccumulateDefault(a, 1)  ==>  PartialSum(a, 1, b);
*/

#endif /* PARTIALSUM_ACSL_INCLUDED */



/*@
  requires   valid:       \valid_read(a + (0..n-1));
  requires   valid:       \valid(b + (0..n-1));
  requires   sep:         \separated(a + (0..n-1), b + (0..n-1));
  requires   bounds:      AccumulateDefaultBounds(a, n);

  terminates              \true;
  exits                   \false;
  assigns                 b[0..n-1];

  ensures    result:      \result == n;
  ensures    partialsum:  PartialSum(a, n, b);
  ensures    unchanged:   Unchanged{Old,Here}(a, n);
*/
size_type partial_sum(const value_type* a, size_type n, value_type* b);

#endif /* PARTIAL_SUM_H_INCLUDED */


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


/* ===== include: DefaultBounds.acsl ===== */

#ifndef DEFAULTBOUNDS_ACSL_INCLUDED
#define DEFAULTBOUNDS_ACSL_INCLUDED

/* could not resolve: #include "limits.h" */

/*@
  predicate DefaultBounds{L}(value_type* a, integer n) =
    \forall integer i; 0 <= i < n  ==>
      VALUE_TYPE_MIN <= a[i] <= VALUE_TYPE_MAX;
*/

#endif /* DEFAULTBOUNDS_ACSL_INCLUDED */


/* ===== include: NumericInverse.acsl ===== */


#ifndef NUMERICINVERSE_ACSL_INCLUDED
#define NUMERICINVERSE_ACSL_INCLUDED


/*@
  lemma PartialSum_Inverse:
    \forall value_type *a, *b, integer n;
      0 <= n               ==>
      PartialSum(a, n, b)  ==>
      AdjacentDifference(b, n, a);

  lemma AdjacentDifference_Inverse:
    \forall value_type *a, *b, integer n;
      0 <= n                       ==>
      AdjacentDifference(a, n, b)  ==>
      PartialSum(b, n, a);

  lemma AdjacentDifference_InverseBounds:
    \forall value_type *a, *b, integer n;
      0 <= n                       ==>
      DefaultBounds(a, n)          ==>
      AdjacentDifference(a, n, b)  ==>
      AccumulateDefaultBounds(b, n);
*/

#endif /* NUMERICINVERSE_ACSL_INCLUDED */



/*@
  requires   size:       0 <= n;
  requires   valid:      \valid(a + (0..n-1));
  requires   valid:      \valid(b + (0..n-1));
  requires   sep:        \separated(a + (0..n-1), b + (0..n-1));
  requires   bounds:     DefaultBounds(a, n);
  requires   bounds:     AdjacentDifferenceBounds(a, n);

  terminates             \true;
  exits                  \false;
  assigns                a[0..n-1], b[0..n-1];

  ensures    unchanged:  Unchanged{Old,Here}(a, n);
*/
void adjacent_difference_inv(value_type* a, size_type n, value_type* b)
{
  adjacent_difference(a, n, b);
  partial_sum(b, n, a);
}

