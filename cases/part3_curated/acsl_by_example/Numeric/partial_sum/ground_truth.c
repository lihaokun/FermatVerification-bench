/* ===== include: partial_sum.c ===== */

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



size_type partial_sum(const value_type* a, size_type n, value_type* b)
{
  if (0u < n) {
    //@ assert limits:      AccumulateDefaultBounds(a, n);
    b[0u] = a[0u];
    //@ assert unchanged:   Unchanged{Pre,Here}(a, n);
    //@ assert limits:      AccumulateDefaultBounds(a, n);
    //@ assert accumulate:  b[0] == AccumulateDefault(a, 1);
    //@ assert partialsum:  PartialSum(a, 1, b);

    /*@
       loop invariant bound:      1 <= i <= n;
       loop invariant unchanged:  Unchanged{Pre,Here}(a, n);
       loop invariant accumulate: b[i-1] == AccumulateDefault(a, i);
       loop invariant limits:     AccumulateDefaultBounds(a, n);
       loop invariant partialsum: PartialSum(a, i, b);
       loop assigns i, b[1..n-1];
       loop variant n - i;
    */
    for (size_type i = 1u; i < n; ++i) {
      b[i] = b[i - 1u] + a[i];
      //@ assert unchanged:  Unchanged{LoopCurrent,Here}(b, i);
      //@ assert unchanged:  Unchanged{LoopCurrent,Here}(a, n);
      //@ assert partialsum: b[i] == AccumulateDefault(a, i+1);
      //@ assert limits:     AccumulateDefaultBounds(a, n);
    }
  }

  return n;
}

