/* ===== include: insertion_sort.c ===== */

/* ===== include: insertion_sort.h ===== */

#ifndef INSERTION_SORT_H_INCLUDED
#define INSERTION_SORT_H_INCLUDED

/* ===== include: MultisetReorder.acsl ===== */

#ifndef MULTISETUNCHANGED_ACSL_INCLUDED
#define MULTISETUNCHANGED_ACSL_INCLUDED

/* ===== include: Count.acsl ===== */

#ifndef COUNT_ACSL_INCLUDED
#define COUNT_ACSL_INCLUDED

/* ===== include: Equal.acsl ===== */

#ifndef EQUAL_ACSL_INCLUDED
#define EQUAL_ACSL_INCLUDED

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
  predicate Equal{K,L}(value_type* a, integer m, integer n, value_type* b) =
    \forall integer i; m <= i < n  ==>  \at(a[i],K) == \at(b[i],L);

  predicate Equal{K,L}(value_type* a, integer n, value_type* b) =
    Equal{K,L}(a, 0, n, b);

  predicate Equal{K,L}(value_type* a, integer m, integer n,
                       value_type* b, integer p) =
    \forall integer k; 0 <= k < n-m ==> \at(a[m+k],K) == \at(b[p+k],L);

  predicate Equal{K,L}(value_type* a, integer m, integer n, integer p) =
      Equal{K,L}(a, m, n, a, p);
*/

#endif /* EQUAL_ACSL_INCLUDED */


/* ===== include: UnchangedLemmas.acsl ===== */

#ifndef UNCHANGEDLEMMAS_ACSL_INCLUDED
#define UNCHANGEDLEMMAS_ACSL_INCLUDED

/* ===== include: Unchanged.acsl ===== */

#ifndef UNCHANGED_ACSL_INCLUDED
#define UNCHANGED_ACSL_INCLUDED


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



/*@
  logic integer
  Count(value_type* a, integer m, integer n, value_type v) =
    n <= m ? 0 : Count(a, m, n-1, v) + (a[n-1] == v ? 1 : 0);

  logic integer
  Count(value_type* a, integer n, value_type v) = Count(a, 0, n, v);

  lemma Count_Empty:
    \forall value_type *a, v, integer m, n;
      n <= m  ==>  Count(a, m, n, v) == 0;

  lemma Count_Hit:
    \forall value_type *a, v, integer n, m;
      m < n        ==>
      a[n-1] == v  ==>  Count(a, m, n, v) == Count(a, m, n-1, v) + 1;

  lemma Count_Miss:
    \forall value_type *a, v, integer n, m;
      m < n        ==>
      a[n-1] != v  ==>  Count(a, m, n, v) == Count(a, m, n-1, v);

  lemma Count_One:
    \forall value_type *a, v, integer m, n;
      m <= n  ==>
     Count(a, m, n+1, v) == Count(a, m, n, v) + Count(a, n, n+1, v);

  lemma Count_Single{K,L}:
    \forall value_type *a, *b, v, integer m, n;
      \at(a[m],K) == \at(b[n],L)  ==>
      Count{K}(a, m, m+1, v) == Count{L}(b, n, n+1, v);

  lemma Count_Equal{K,L}:
    \forall value_type *a, v, integer m, n, p;
      0 <= m <= n             ==>
      Equal{K,L}(a, m, n, p)  ==>
      Count{K}(a, m, n, v) == Count{L}(a, p, p + (n-m), v);

  lemma Count_Unchanged{K,L}:
    \forall value_type *a, v, integer m, n;
      Unchanged{K,L}(a, m, n)  ==>
      Count{K}(a, m, n, v) == Count{L}(a, m, n, v);

  lemma Count_Union:
    \forall value_type *a, v, integer k, m, n;
      0 <= k <= m <= n  ==>
      Count(a, k, n, v) == Count(a, k, m, v) + Count(a, m, n, v);

  lemma Count_Cut:
    \forall value_type *a, v, integer k, m, n;
      0 <= k <= m < n  ==> Count(a, k, n, v) ==
        Count(a, k, m, v) + Count(a, m, m+1, v) + Count(a, m+1, n, v);

  lemma Count_Single_Bounds:
    \forall value_type *a, v, integer n;
      0 <= Count(a, n, n+1, v) <= 1;

  lemma Count_Bounds:
    \forall value_type *a, v, integer m, n;
      0 <= m <= n  ==>  0 <= Count(a, m, n, v) <= n-m;

  lemma Count_Increasing:
    \forall value_type *a, v, integer m, n, p;
      m <= n <= p  ==>  Count(a, m, n, v) <= Count(a, m, p, v);

  lemma Count_Single_Shift:
    \forall value_type *a, v, integer n;
      0 <= n  ==>  Count(a+n, 0, 1, v) == Count(a, n, n+1, v);

  lemma Count_Shift:
    \forall value_type *a, v, integer m, n;
      0 <= m  ==>  0 <= n  ==>  Count(a+m, 0, n, v) == Count(a, m, m+n, v);
*/

#endif /* COUNT_ACSL_INCLUDED */



/*@
  predicate MultisetReorder{K,L}(value_type* a, integer m, integer n) =
    \forall value_type v;
      Count{K}(a, m, n, v) == Count{L}(a, m, n, v);

  predicate MultisetReorder{K,L}(value_type* a, integer n) =
    MultisetReorder{K,L}(a, 0, n);

  lemma Unchanged_MultisetReorder{K,L}:
    \forall value_type *a, integer k, n;
      Unchanged{K,L}(a, k, n)  ==>  MultisetReorder{K,L}(a, k, n);

  lemma MultisetReorder_DisjointUnion{K,L}:
    \forall value_type *a, integer i, k, n;
      0 <= i <= k <= n               ==>
      MultisetReorder{K,L}(a, i, k)  ==>
      MultisetReorder{K,L}(a, k, n)  ==>
      MultisetReorder{K,L}(a, i, n);

  lemma MultisetReorder_Symmetric{K,L}:
    \forall value_type *a, integer m, n;
      MultisetReorder{K,L}(a, m, n)  ==>  MultisetReorder{L,K}(a, m, n);

  lemma MultisetReorder_Transitive{K,L,M}:
    \forall value_type *a, integer m, n;
      MultisetReorder{K,L}(a, m, n)  ==>
      MultisetReorder{L,M}(a, m, n)  ==>
      MultisetReorder{K,M}(a, m, n);
*/

#endif /* MULTISETUNCHANGED_ACSL_INCLUDED */


/* ===== include: Increasing.acsl ===== */

#ifndef INCREASING_ACSL_INCLUDED
#define INCREASING_ACSL_INCLUDED

/* ===== include: WeaklyIncreasing.acsl ===== */

#ifndef WEAKLYINCREASING_ACSL_INCLUDED
#define WEAKLYINCREASING_ACSL_INCLUDED


/*@
  predicate WeaklyIncreasing{L}(value_type* a, integer m, integer n) =
    \forall integer i; m <= i < n-1  ==>  a[i] <= a[i+1];

  predicate WeaklyIncreasing{L}(value_type* a, integer n) = WeaklyIncreasing{L}(a, 0, n);
*/


#endif /* WEAKLYINCREASING_ACSL_INCLUDED */



/*@
  predicate Increasing{L}(value_type* a, integer m, integer n) =
    \forall integer i, j; m <= i < j < n  ==>  a[i] <= a[j];

  predicate Increasing{L}(value_type* a, integer n) = Increasing{L}(a, 0, n);
*/


#endif /* INCREASING_ACSL_INCLUDED */



/*@
  requires   valid:       \valid(a + (0..n-1));

  terminates              \true;
  exits                   \false;
  assigns                 a[0..n-1];

  ensures    reorder:     MultisetReorder{Old,Here}(a, n);
  ensures    increasing:  Increasing(a, n);
*/
void insertion_sort(value_type* a, size_type n);

#endif /* INSERTION_SORT_H_INCLUDED */


/* could not resolve: #include "upper_bound.h" */
/* could not resolve: #include "rotate.h" */
/* ===== include: ArrayBounds.acsl ===== */

#ifndef ARRAYBOUNDS_ACSL_INCLUDED
#define ARRAYBOUNDS_ACSL_INCLUDED

/* ===== include: LessThanComparable.acsl ===== */

#ifndef LESSTHANCOMPARABLE_ACSL_INCLUDED
#define LESSTHANCOMPARABLE_ACSL_INCLUDED


/*@
  lemma Less_Irreflexivity:
    \forall value_type a; !(a < a);

  lemma Less_Antisymmetry:
    \forall value_type a, b; (a < b)     ==>  !(b < a);

  lemma Less_Transitivity:
    \forall value_type a, b, c; (a < b)  ==>  (b < c)  ==>  (a < c);

  lemma Greater_Less:
    \forall value_type a, b;  (a > b)   <==>  (b < a);

  lemma LessOrEqual_Less:
    \forall value_type a, b;  (a <= b)  <==>  !(b < a);

  lemma GreaterOrEqual_Less:
    \forall value_type a, b;  (a >= b)  <==> !(a < b);
*/

#endif /* LESSTHANCOMPARABLE_ACSL_INCLUDED */



/*@
    predicate LowerBound{L}(value_type* a, integer m, integer n, value_type v) =
      \forall integer i; m <= i < n  ==>  v <= a[i];

    predicate LowerBound{L}(value_type* a, integer n, value_type v) =
      LowerBound{L}(a, 0, n, v);

    predicate StrictLowerBound{L}(value_type* a, integer m, integer n, value_type v) =
      \forall integer i; m <= i < n  ==>  v < a[i];

    predicate StrictLowerBound{L}(value_type* a, integer n, value_type v) =
      StrictLowerBound{L}(a, 0, n, v);

    predicate UpperBound{L}(value_type* a, integer m, integer n, value_type v) =
      \forall integer i; m <= i < n  ==>  a[i] <= v;

    predicate UpperBound{L}(value_type* a, integer n, value_type v) =
      UpperBound{L}(a, 0, n, v);

    predicate StrictUpperBound{L}(value_type* a, integer m, integer n, value_type v) =
      \forall integer i; m <= i < n  ==>  a[i] < v;

    predicate StrictUpperBound{L}(value_type* a, integer n, value_type v) =
      StrictUpperBound{L}(a, 0, n, v);
*/

#endif /* ARRAYBOUNDS_ACSL_INCLUDED */


/* ===== include: RotateLemmas.acsl ===== */

#ifndef ROTATELEMMAS_ACSL_INCLUDED
#define ROTATELEMMAS_ACSL_INCLUDED

/* ===== include: Rotate.acsl ===== */

#ifndef ROTATE_ACSL_INCLUDED
#define ROTATE_ACSL_INCLUDED


/*@
  predicate Rotate{K,L}(value_type* a, integer p, integer n, value_type* b) =
     Equal{K,L}(a, 0, p, b, n-p) &&
     Equal{K,L}(a, p, n, b, 0);

  predicate Rotate{K,L}(value_type* a, integer m, integer p, integer n) =
    Equal{K,L}(a, m, p, m+n-p) &&
    Equal{K,L}(a, p, n, m);

  predicate Rotate{K,L}(value_type* a, integer p, integer n) =
    Rotate{K,L}(a, 0, p, n);
*/

#endif /* ROTATE_ACSL_INCLUDED */


/* ===== include: IncreasingLemmas.acsl ===== */

#ifndef INCREASINGLEMMAS_ACSL_INCLUDED
#define INCREASINGLEMMAS_ACSL_INCLUDED


/*@
  lemma Increasing_Shift{L}:
    \forall value_type *a, integer l, r;
      0 <= l <= r             ==>
      Increasing{L}(a, l, r)  ==>
      Increasing{L}(a+l, r-l);

  lemma Increasing_Equal{K,L}:
    \forall value_type* a, integer m, n, p;
      Increasing{K}(a, m, n)    ==>
      Equal{K,L}(a, m, n, m+p)  ==>
      Increasing{L}(a, m+p, n+p);

  lemma WeaklyIncreasing_Increasing{L}:
    \forall value_type* a, integer m, n;
      0 <= m <= n                ==>
      WeaklyIncreasing(a, m, n)  ==>
      Increasing(a, m, n);

  lemma Increasing_WeaklyIncreasing{L}:
    \forall value_type* a, integer m, n;
      0 <= m <= n                 ==>
      Increasing(a, m, n)         ==>
      WeaklyIncreasing(a, m, n);
*/

#endif /* INCREASINGLEMMAS_ACSL_INCLUDED */




/*@
  lemma Rotate_Shift{K,L}:
    \forall value_type *a, integer k, m, p, n;
      Rotate{K,L}(a+k, m, p, n)  ==>  Rotate{K,L}(a, m+k, p+k, n+k);

  lemma RotateOne_StrictLowerBound{K,L}:
    \forall value_type* a, integer m, n;
      StrictLowerBound{K}(a, m,   n,   \at(a[n],K))  ==>
      Rotate{K,L}(a, m, n, n+1)                      ==>
      StrictLowerBound{L}(a, m+1, n+1, \at(a[m],L));

  lemma RotateOne_MultisetReorder{K,L}:
    \forall value_type* a, integer m, n;
      0 <= m <= n                ==>
      Rotate{K,L}(a, m, n, n+1)  ==>
      MultisetReorder{K,L}(a, m, n+1);
*/

#endif /* ROTATELEMMAS_ACSL_INCLUDED */



void insertion_sort(value_type* a, size_type n)
{
  /*@
    loop invariant bound:         0 <= i <= n;
    loop invariant reorder:       MultisetReorder{Pre,Here}(a, 0, i);
    loop invariant unchanged:     Unchanged{Pre,Here}(a, i, n);
    loop invariant increasing:    WeaklyIncreasing(a, i);
    loop assigns   i, a[0..n-1];
    loop variant   n - i;
  */
  for (size_type i = 0u; i < n; ++i) {
    const size_type k = upper_bound(a, i, a[i]);
    //@ assert bound:   0 <= k <= i;
    //@ ghost Before: ;
    //@ assert lower:       StrictLowerBound(a, k, i, a[i]);
    //@ assert upper:       UpperBound(a, k, a[i]);
    rotate(a + k, i - k, i + 1u - k);
    //@ assert rotate:      Rotate{Before,Here}(a, k, i, i+1);
    //@ assert upper:       UpperBound(a, k, a[k]);
    //@ assert increasing:  WeaklyIncreasing(a, k+1);
    //@ assert increasing:  WeaklyIncreasing(a, k+1, i+1);
    //@ assert increasing:  WeaklyIncreasing(a, i+1);
    //@ assert reorder:     MultisetReorder{LoopCurrent,Here}(a, 0, k);
    //@ assert reorder:     MultisetReorder{LoopCurrent,Here}(a, k, i+1);
    //@ assert reorder:     MultisetReorder{LoopCurrent,Here}(a, 0, i+1);
    //@ assert reorder:     MultisetReorder{Pre,Here}(a, i+1);
  }

  //@ assert increasing: Increasing(a, n);
}

