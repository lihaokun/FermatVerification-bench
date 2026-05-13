/* ===== include: minmax_element.c ===== */

/* ===== include: minmax_element.h ===== */

#ifndef MINMAX_ELEMENT_H_INCLUDED
#define MINMAX_ELEMENT_H_INCLUDED

/* ===== include: SizeTypePairTrivial.acsl ===== */

#ifndef SIZETYPEPAIR_ACSL_INCLUDED
#define SIZETYPEPAIR_ACSL_INCLUDED

// dummy specification file that includes size_type_pair.h

/* ===== include: size_type_pair.h ===== */

#ifndef SIZE_TYPE_PAIR_H_INCLUDED
#define SIZE_TYPE_PAIR_H_INCLUDED

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



struct size_type_pair {
  size_type first;
  size_type second;
};

typedef struct size_type_pair size_type_pair;

#endif /* SIZE_TYPE_PAIR_H_INCLUDED */



#endif  /* SIZETYPEPAIR_ACSL_INCLUDED */


/* ===== include: ArrayExtrema.acsl ===== */

#ifndef ARRAYEXTREMA_ACSL_INCLUDED
#define ARRAYEXTREMA_ACSL_INCLUDED

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
  predicate MaxElement{L}(value_type* a, integer n, integer max) =
    0 <= max < n && UpperBound(a, n, a[max]);

  predicate MinElement{L}(value_type* a, integer n, integer min) =
    0 <= min < n && LowerBound(a, n, a[min]);
*/

#endif /* ARRAYEXTREMA_ACSL_INCLUDED */



/*@
  requires   valid:  \valid_read(a + (0..n-1));

  terminates         \true;
  exits              \false;
  assigns            \nothing;

  ensures    result:  0 <= \result.first  <= n;
  ensures    result:  0 <= \result.second <= n;

  behavior empty:
    assumes          0 == n;
    assigns          \nothing;
    ensures result:  \result.first == 0;
    ensures result:  \result.second == 0;

  behavior not_empty:
    assumes          0 < n;
    assigns          \nothing;
    ensures result:  0 <= \result.first < n;
    ensures result:  0 <= \result.second < n;

    ensures min:     MinElement(a, n, \result.first);
    ensures first:   StrictLowerBound(a, \result.first, a[\result.first]);
    ensures max:     MaxElement(a, n, \result.second);
    ensures last:    StrictUpperBound(a, \result.second+1, n, a[\result.second]);
*/
size_type_pair minmax_element(const value_type* a, size_type n);

#endif /* MINMAX_ELEMENT_H_INCLUDED */


/* ===== include: make_pair.h ===== */

#ifndef MAKE_PAIR_H_INCLUDED
#define MAKE_PAIR_H_INCLUDED


/*@
    terminates         \true;
    exits              \false;
    assigns            \nothing;

    ensures   result:  \result.first  == first;
    ensures   result:  \result.second == second;
*/
size_type_pair make_pair(size_type first, size_type second);

#endif /* MAKE_PAIR_H_INCLUDED */



size_type_pair minmax_element(const value_type* a, size_type n)
{
  if (0u < n) {
    size_type min = 0u;
    size_type max = 0u;

    /*@
      loop invariant bound: 0 <= i    <= n;
      loop invariant min:   0 <= min  <  n;
      loop invariant max:   0 <= max  <  n;
      loop invariant lower: LowerBound(a, i, a[min]);
      loop invariant upper: UpperBound(a, i, a[max]);
      loop invariant first: StrictLowerBound(a, min, a[min]);
      loop invariant last:  StrictUpperBound(a, max+1, i, a[max]);
      loop assigns min, max, i;

      loop variant n-i;
    */
    for (size_type i = 0u; i < n; i++) {
      if (a[i] >= a[max]) {
        max = i;
      }

      if (a[i] < a[min]) {
        min = i;
      }
    }

    return make_pair(min, max);
  }

  return make_pair(n, n);
}

