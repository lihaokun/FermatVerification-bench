/* ===== include: find_first_of.c ===== */

/* ===== include: find_first_of.h ===== */

#ifndef FIND_FIRST_OF_H_INCLUDED
#define FIND_FIRST_OF_H_INCLUDED

/* ===== include: HasValueOf.acsl ===== */

#ifndef HASVALUEOF_ACSL_INCLUDED
#define HASVALUEOF_ACSL_INCLUDED

/* ===== include: SomeNone.acsl ===== */

#ifndef SOMENONE_ACSL_INCLUDED
#define SOMENONE_ACSL_INCLUDED

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
  predicate SomeEqual{A}(value_type* a, integer m, integer n, value_type v) =
    \exists integer i; m <= i < n && a[i] == v;

  predicate SomeEqual{A}(value_type* a, integer n, value_type v) =
    SomeEqual(a, 0, n, v);

  predicate NoneEqual(value_type* a, integer m, integer n, value_type v) =
    \forall integer i; m <= i < n  ==>  a[i] != v;

  predicate NoneEqual(value_type* a, integer n, value_type v) =
    NoneEqual(a, 0, n, v);

  lemma NotSomeEqual_NoneEqual:
    \forall value_type *a, v, integer m, n;
      !SomeEqual(a, m, n, v)  ==>  NoneEqual(a, m, n, v);

  lemma NoneEqual_NotSomeEqual:
    \forall value_type *a, v, integer m, n;
     NoneEqual(a, m, n, v)   ==>  !SomeEqual(a, m, n, v);
*/

#endif /* SOMENONE_ACSL_INCLUDED */



/*@
  predicate HasValueOf{A}(value_type* a, integer m, value_type* b, integer n) =
    \exists integer i; 0 <= i < m && SomeEqual{A}(b, n, a[i]);
*/

#endif /* HASVALUEOF_ACSL_INCLUDED */



/*@
  requires   valid:  \valid_read(a + (0..m-1));
  requires   valid:  \valid_read(b + (0..n-1));

  terminates        \true;
  exits             \false;
  assigns           \nothing;

  ensures    result:   0 <= \result <= m;

  behavior found:
    assumes          HasValueOf(a, m, b, n);
    assigns          \nothing;
    ensures bound:   0 <= \result < m;
    ensures result:  SomeEqual(b, n, a[\result]);
    ensures first:   !HasValueOf(a, \result, b, n);

  behavior not_found:
    assumes          !HasValueOf(a, m, b, n);
    assigns          \nothing;
    ensures result:  \result == m;

  complete behaviors;
  disjoint behaviors;
*/
size_type find_first_of(const value_type* a, size_type m,
                        const value_type* b, size_type n);

#endif /* FIND_FIRST_OF_H_INCLUDED */

/* ===== include: find2.h ===== */

#ifndef FIND2_H_INCLUDED
#define FIND2_H_INCLUDED


/*@
  requires   valid:   \valid_read(a + (0..n-1));

  terminates          \true;
  exits               \false;
  assigns             \nothing;

  ensures    result:  0 <= \result <= n;

  behavior some:
    assumes           SomeEqual(a, n, v);
    assigns           \nothing;
    ensures  bound:   0 <= \result < n;
    ensures  result:  a[\result] == v;
    ensures  first:   NoneEqual(a, \result, v);

  behavior none:
    assumes           NoneEqual(a, n, v);
    assigns           \nothing;
    ensures  result:  \result == n;

  complete behaviors;
  disjoint behaviors;
*/
size_type find2(const value_type* a, size_type n, value_type v);

#endif /* FIND2_H_INCLUDED */



size_type find_first_of (const value_type* a, size_type m,
                         const value_type* b, size_type n)
{
  /*@
    loop invariant bound:      0 <= i <= m;
    loop invariant not_found: !HasValueOf(a, i, b, n);
    loop assigns i;
    loop variant m-i;
  */
  for (size_type i = 0u; i < m; i++) {
    if (find2(b, n, a[i]) < n) {
      return i;
    }
  }

  return m;
}

