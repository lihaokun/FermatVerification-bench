/* ===== include: equal_range2.c ===== */

/* ===== include: equal_range2.h ===== */

#ifndef EQUAL_RANGE_H_INCLUDED
#define EQUAL_RANGE_H_INCLUDED

/* ===== include: SizeTypePairTrivial.acsl ===== */

#ifndef SIZETYPEPAIR_ACSL_INCLUDED
#define SIZETYPEPAIR_ACSL_INCLUDED

// dummy specification file that includes size_type_pair.h

/* could not resolve: #include "size_type_pair.h" */

#endif  /* SIZETYPEPAIR_ACSL_INCLUDED */


/* ===== include: Increasing.acsl ===== */

#ifndef INCREASING_ACSL_INCLUDED
#define INCREASING_ACSL_INCLUDED

/* ===== include: WeaklyIncreasing.acsl ===== */

#ifndef WEAKLYINCREASING_ACSL_INCLUDED
#define WEAKLYINCREASING_ACSL_INCLUDED

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


/* ===== include: AllSomeNot.acsl ===== */

#ifndef ALLSOMENOT_ACSL_INCLUDED
#define ALLSOMENOT_ACSL_INCLUDED


/*@
    predicate AllEqual(value_type* a, integer m, integer n, value_type v) =
      \forall integer i; m <= i < n  ==>  a[i] == v;

    predicate AllEqual(value_type* a, integer m, integer n) =
      AllEqual(a, m, n, a[m]);

    predicate AllEqual(value_type* a, integer n, value_type v) =
      AllEqual(a, 0, n, v);

    predicate SomeNotEqual{A}(value_type* a, integer m, integer n, value_type v) =
      \exists integer i; m <= i < n && a[i] != v;

    predicate SomeNotEqual{A}(value_type* a, integer n, value_type v) =
      SomeNotEqual(a, 0, n, v);

    lemma NotAllEqual_SomeNotEqual:
      \forall value_type *a, v, integer m, n;
        !AllEqual(a, m, n, v)  ==>  SomeNotEqual(a, m, n, v);

    lemma SomeNotEqual_NotAllEqual:
      \forall value_type *a, v, integer m, n;
       SomeNotEqual(a, m, n, v)   ==>  !AllEqual(a, m, n, v);
*/

#endif /* ALLSOMENOT_ACSL_INCLUDED */


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



/*@
  requires   valid:       \valid_read(a + (0..n-1));
  requires   increasing:  Increasing(a, n);

  terminates              \true;
  exits                   \false;
  assigns                 \nothing;

  ensures    result:      0 <= \result.first <= \result.second <= n;
  ensures    left:        StrictUpperBound(a, 0, \result.first, v);
  ensures    middle:      AllEqual(a, \result.first, \result.second, v);
  ensures    right:       StrictLowerBound(a, \result.second, n, v);
 */
size_type_pair equal_range2(const value_type* a, size_type n, value_type v);

#endif /* EQUAL_RANGE_H_INCLUDED */


/* ===== include: lower_bound.h ===== */

#ifndef LOWER_H_BOUND_INCLUDED
#define LOWER_H_BOUND_INCLUDED


/*@
  requires   valid:       \valid_read(a + (0..n-1));
  requires   increasing:  Increasing(a, n);

  terminates              \true;
  exits                   \false;
  assigns                 \nothing;

  ensures    result:      0 <= \result <= n;
  ensures    left:        StrictUpperBound(a, 0, \result, v);
  ensures    right:       LowerBound(a, \result, n, v);
*/
size_type lower_bound(const value_type* a, size_type n, value_type v);

#endif /* LOWER_H_BOUND_INCLUDED */


/* ===== include: upper_bound.h ===== */

#ifndef UPPER_BOUND_H_INCLUDED
#define UPPER_BOUND_H_INCLUDED


/*@
  requires   valid:       \valid_read(a + (0..n-1));
  requires   increasing:  Increasing(a, n);

  terminates              \true;
  exits                   \false;
  assigns                 \nothing;

  ensures    result:      0 <= \result <= n;
  ensures    left:        UpperBound(a, 0, \result, v);
  ensures    right:       StrictLowerBound(a, \result, n, v);
*/
size_type upper_bound(const value_type* a, size_type n, value_type v);

#endif /* UPPER_BOUND_H_INCLUDED */


/* could not resolve: #include "make_pair.h" */
/* ===== include: ArrayBoundsShift.acsl ===== */
#ifndef SHIFTLEMMAS_ACSL_INCLUDED
#define SHIFTLEMMAS_ACSL_INCLUDED


/*@
  lemma LowerBound_Shift{L}:
    \forall value_type *a, v, integer b, c, d;
      LowerBound{L}(a+b, c, d, v)  ==>  LowerBound{L}(a, c+b, d+b, v);

  lemma StrictLowerBound_Shift{L}:
    \forall value_type *a, v, integer b, c, d;
      StrictLowerBound{L}(a+b, c, d, v)  ==> StrictLowerBound{L}(a, c+b, d+b, v);

  lemma UpperBound_Shift{L}:
    \forall value_type *a, v, integer b, c;
      UpperBound{L}(a+b, 0, c-b, v)  ==>  UpperBound{L}(a, b, c, v);

  lemma StrictUpperBound_Shift{L}:
    \forall value_type *a, v, integer b, c;
      StrictUpperBound{L}(a+b, 0, c-b, v)  ==> StrictUpperBound{L}(a, b, c, v);
*/

#endif /* SHIFTLEMMAS_ACSL_INCLUDED */

/* ===== include: IncreasingLemmas.acsl ===== */

#ifndef INCREASINGLEMMAS_ACSL_INCLUDED
#define INCREASINGLEMMAS_ACSL_INCLUDED

/* ===== include: Equal.acsl ===== */

#ifndef EQUAL_ACSL_INCLUDED
#define EQUAL_ACSL_INCLUDED


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



size_type_pair equal_range2(const value_type* a, size_type n, value_type v)
{
  size_type first  = 0u;
  size_type middle = 0u;
  size_type last   = n;

  /*@
    loop invariant bounds: 0 <= first <= last <= n;
    loop invariant left:   StrictUpperBound(a, 0, first, v);
    loop invariant right:  StrictLowerBound(a, last, n, v);
    loop assigns first, last, middle;
    loop variant last - first;
   */
  while (last > first) {
    middle = first + (last - first) / 2u;

    if (a[middle] < v) {
      first = middle + 1u;
    }
    else if (v < a[middle]) {
      last = middle;
    }
    else {
      break;
    }
  }

  if (first < last) {
    //@ assert increasing: Increasing(a, first, middle);
    size_type left = first + lower_bound(a + first, middle - first, v);
    //@ assert middle: LowerBound(a, left, middle, v);
    //@ assert left:   StrictUpperBound(a, first, left, v);
    ++middle;
    //@ assert increasing: Increasing(a, middle, last);
    size_type right = middle + upper_bound(a + middle, last - middle, v);
    //@ assert middle: UpperBound(a, middle, right, v);
    //@ assert right:  StrictLowerBound(a, right, last, v);
    //@ assert middle: AllEqual(a, left, right, v);
    return make_pair(left, right);
  }
  else {
    return make_pair(first, first);
  }
}

