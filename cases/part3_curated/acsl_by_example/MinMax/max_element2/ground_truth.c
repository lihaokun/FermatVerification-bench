/* ===== include: max_element2.c ===== */

/* ===== include: max_element2.h ===== */

#ifndef MAX_ELEMENT2_H_INCLUDED
#define MAX_ELEMENT2_H_INCLUDED

/* ===== include: ArrayExtrema.acsl ===== */

#ifndef ARRAYEXTREMA_ACSL_INCLUDED
#define ARRAYEXTREMA_ACSL_INCLUDED

/* ===== include: ArrayBounds.acsl ===== */

#ifndef ARRAYBOUNDS_ACSL_INCLUDED
#define ARRAYBOUNDS_ACSL_INCLUDED

/* ===== include: LessThanComparable.acsl ===== */

#ifndef LESSTHANCOMPARABLE_ACSL_INCLUDED
#define LESSTHANCOMPARABLE_ACSL_INCLUDED

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
  predicate MaxElement{L}(value_type* a, integer n, integer max) =
    0 <= max < n && UpperBound(a, n, a[max]);

  predicate MinElement{L}(value_type* a, integer n, integer min) =
    0 <= min < n && LowerBound(a, n, a[min]);
*/

#endif /* ARRAYEXTREMA_ACSL_INCLUDED */



/*@
  requires   valid:   \valid_read(a + (0..n-1));

  terminates          \true;
  exits               \false;
  assigns             \nothing;

  ensures    result:  0 <= \result <= n;

  behavior empty:
    assumes          n == 0;
    assigns          \nothing;
    ensures result:  \result == 0;

  behavior not_empty:
    assumes          0 < n;
    assigns          \nothing;
    ensures result:  0 <= \result < n;
    ensures max:     MaxElement(a, n, \result);
    ensures first:   StrictUpperBound(a, \result, a[\result]);

  complete behaviors;
  disjoint behaviors;
*/
size_type max_element2(const value_type* a, size_type n);

#endif /* MAX_ELEMENT2_H_INCLUDED */


size_type max_element2(const value_type* a, size_type n)
{
  if (0u < n) {
    size_type max = 0u;

    /*@
      loop invariant bound:   0 <= i <= n;
      loop invariant max:     0 <= max < n;
      loop invariant upper:   UpperBound(a, i, a[max]);
      loop invariant first:   StrictUpperBound(a, max, a[max]);
      loop assigns max, i;
      loop variant n-i;
    */
    for (size_type i = 0u; i < n; i++) {
      if (a[max] < a[i]) {
        max = i;
      }
    }

    return max;
  }

  return n;
}

