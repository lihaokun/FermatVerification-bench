/* ===== include: fill.c ===== */

/* ===== include: fill.h ===== */

#ifndef FILL_H_INCLUDED
#define FILL_H_INCLUDED

/* ===== include: AllSomeNot.acsl ===== */

#ifndef ALLSOMENOT_ACSL_INCLUDED
#define ALLSOMENOT_ACSL_INCLUDED

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



/*@
  requires   valid:   \valid(a + (0..n-1));

  terminates           \true;
  exits                \false;
  assigns              a[0..n-1];

  ensures    constant: AllEqual(a, n, v);
*/
void fill(value_type* a, size_type n, value_type v);

#endif /* FILL_H_INCLUDED */



void fill(value_type* a, size_type n,  value_type v)
{
  /*@
    loop invariant bound:    0 <= i <= n;
    loop invariant constant: AllEqual(a, i, v);
    loop assigns i, a[0..n-1];
    loop variant n-i;
  */
  for (size_type i = 0u; i < n; ++i) {
    a[i] = v;
  }
}

