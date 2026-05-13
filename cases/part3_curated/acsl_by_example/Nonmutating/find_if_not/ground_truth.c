/* ===== include: find_if_not.c ===== */

/* ===== include: find_if_not.h ===== */

#ifndef FINDIFNOT_H_INCLUDED
#define FINDIFNOT_H_INCLUDED

/* ===== include: FindNotEqual.acsl ===== */

#ifndef FINDNOTEQUAL_ACSL_INCLUDED
#define FINDNOTEQUAL_ACSL_INCLUDED

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
  FindNotEqual(value_type* a, integer m, integer n, value_type v) =
    (n <= m) ? 
     0 : ((0 <= FindNotEqual(a, m, n-1, v) < n-m-1) ?
       FindNotEqual(a, m, n-1, v) : ((a[n-1] != v) ? n-m-1 : n-m));

  logic integer
  FindNotEqual(value_type* a, integer n, value_type v) =
    FindNotEqual(a, 0, n, v);

  lemma FindNotEqual_Empty:
    \forall value_type *a, v, integer m, n;
      n <= m  ==>  FindNotEqual(a, m, n, v) == 0;

  lemma FindNotEqual_Hit:
    \forall value_type *a, v, integer m, n;
      m <= n  ==>  FindNotEqual(a, m, n, v) < n-m  ==>
      FindNotEqual(a, m, n+1, v) == FindNotEqual(a, m, n, v);

  lemma FindNotEqual_MissHit:
    \forall value_type *a, v, integer m, n;
      m <= n  ==>  a[n] != v             ==>
      FindNotEqual(a, m, n, v)   == n-m  ==>
      FindNotEqual(a, m, n+1, v) == n-m;

  lemma FindNotEqual_MissMiss:
    \forall value_type *a, v, integer m, n;
      m <= n  ==>  a[n] == v             ==>
      FindNotEqual(a, m, n, v)   == n-m  ==>
      FindNotEqual(a, m, n+1, v) == (n+1)-m;

  lemma FindNotEqual_Lower:
    \forall value_type *a, v, integer m, n;
      0 <= FindNotEqual(a, m, n, v);

  lemma FindNotEqual_Upper:
    \forall value_type *a, v, integer m, n;
      m <= n  ==>  FindNotEqual(a, m, n, v) <= n-m;

  lemma FindNotEqual_Unchanged{K,L}:
    \forall value_type *a, v, integer m, n;
      Unchanged{K,L}(a, m, n)  ==>
      FindNotEqual{K}(a, m, n, v) == FindNotEqual{L}(a, m, n, v);

  lemma FindNotEqual_WeaklyIncreasing:
    \forall value_type *a, v, integer m, n;
      m <= n  ==>  FindNotEqual(a, m, n, v) <= FindNotEqual(a, m, n+1, v);

  lemma FindNotEqual_Increasing:
    \forall value_type *a, v, integer k, m, n;
      m <= k <= n  ==>  FindNotEqual(a, m, k, v) <= FindNotEqual(a, m, n, v);

  lemma FindNotEqual_Limit:
    \forall value_type *a, v, integer k, m, n;
      m <= k < n  ==>
      a[k] != v   ==>  FindNotEqual(a, m, n, v) <= k-m;

  lemma FindNotEqual_AllEqual:
    \forall value_type *a, v, integer m, n;
      m <= n                ==>
      AllEqual(a, m, n, v)  ==>  FindNotEqual(a, m, n, v) == n-m;

  lemma FindNotEqual_SomeNotEqual:
    \forall value_type *a, v, integer k, m, n;
      m <= k < n            ==>  a[k] != v  ==>
      AllEqual(a, m, k, v)  ==>  FindNotEqual(a, m, n, v) == k-m;

  lemma FindNotEqual_ResultAllEqual:
    \forall value_type *a, v, integer m, n;
      m <= n  ==>  AllEqual(a, m, m + FindNotEqual(a, m, n, v), v);

  lemma FindNotEqual_ResultNotEqual:
    \forall value_type *a, v, integer m, n;
      0 <= FindNotEqual(a, m, n, v) < n-m  ==>
      a[m + FindNotEqual(a, m, n, v)] != v;
*/

#endif /* FINDNOTEQUAL_ACSL_INCLUDED */



/*@
  requires   valid:   \valid_read(a + (0..n-1));

  terminates          \true;
  exits               \false;
  assigns             \nothing;

  ensures    result:  0 <= \result <= n;
  ensures    result:  \result == FindNotEqual(a, n, v);
*/
size_type find_if_not(const value_type* a, size_type n, value_type v);

#endif /* FINDIFNOT_H_INCLUDED */




size_type find_if_not(const value_type* a, size_type n, value_type v)
{
  /*@
    loop invariant bound:     0 <= i <= n;
    loop invariant not_found: FindNotEqual(a, i, v) == i;
    loop assigns i;
    loop variant n-i;
   */
  for (size_type i = 0u; i < n; i++) {
    if (a[i] != v) {
      //@ assert found: FindNotEqual(a, n, v) == i;
      return i;
    }
  }

  return n;
}

