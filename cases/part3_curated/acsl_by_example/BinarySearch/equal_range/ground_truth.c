/* ===== include: equal_range.c ===== */

/* ===== include: equal_range.h ===== */

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
size_type_pair equal_range(const value_type* a, size_type n, value_type v);

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

size_type_pair equal_range(const value_type* a, size_type n, value_type v)
{
  size_type first  = lower_bound(a, n, v);
  size_type second = upper_bound(a, n, v);
  //@ assert aux: second < n  ==>  v < a[second];
  return make_pair(first, second);
}

