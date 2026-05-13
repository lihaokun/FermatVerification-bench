/* ===== include: find5.c ===== */

/* ===== include: find5.h ===== */

#ifndef FIND5_H_INCLUDED
#define FIND5_H_INCLUDED

/* ===== include: Find.acsl ===== */

#ifndef FIND_ACSL_INCLUDED
#define FIND_ACSL_INCLUDED

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
  logic integer
  Find(value_type* a, integer m, integer n, value_type v) =
    (n <= m) ? 
     0 : ((0 <= Find(a, m, n-1, v) < n-m-1) ?
       Find(a, m, n-1, v) : ((a[n-1] == v) ? n-m-1 : n-m));

  logic integer
  Find(value_type* a, integer n, value_type v) = Find(a, 0, n, v);

  lemma Find_Empty:
    \forall value_type *a, v, integer m, n;
      n <= m  ==>  Find(a, m, n, v) == 0;

  lemma Find_Hit:
    \forall value_type *a, v, integer m, n;
      m <= n                  ==>
      Find(a, m, n, v) < n-m  ==>
      Find(a, m, n+1, v) == Find(a, m, n, v);

  lemma Find_MissHit:
    \forall value_type *a, v, integer m, n;
      m <= n                     ==>
      a[n] == v                  ==>
      Find(a, m, n, v)   == n-m  ==>
      Find(a, m, n+1, v) == n-m;

  lemma Find_MissMiss:
    \forall value_type *a, v, integer m, n;
      m <= n                     ==>
      a[n] != v                  ==>
      Find(a, m, n, v)   == n-m  ==>
      Find(a, m, n+1, v) == (n+1)-m;

  lemma Find_Lower:
    \forall value_type *a, v, integer m, n;
      0 <= Find(a, m, n, v);

  lemma Find_Upper:
    \forall value_type *a, v, integer m, n;
      m <= n  ==>  Find(a, m, n, v) <= n-m;

  lemma Find_WeaklyIncreasing:
    \forall value_type *a, v, integer m, n;
      m <= n  ==>  Find(a, m, n, v) <= Find(a, m, n+1, v);

  lemma Find_Increasing:
    \forall value_type *a, v, integer k, m, n;
      m <= k <= n  ==>
      Find(a, m, k, v) <= Find(a, m, n, v);

  lemma Find_Limit:
    \forall value_type *a, v, integer k, m, n;
      m <= k < n  ==>
      a[k] == v   ==>
      Find(a, m, n, v) <= k-m;

  lemma Find_NoneEqual:
    \forall value_type *a, v, integer m, n;
      m <= n                 ==>
      NoneEqual(a, m, n, v)  ==>
      Find(a, m, n, v) == n-m;

  lemma Find_SomeEqual:
    \forall value_type *a, v, integer k, m, n;
      m <= k < n             ==> 
      a[k] == v              ==>
      NoneEqual(a, m, k, v)  ==> 
      Find(a, m, n, v) == k-m;

  lemma Find_ResultNoneEqual:
    \forall value_type *a, v, integer m, n;
      m <= n  ==>  NoneEqual(a, m, m + Find(a, m, n, v), v);

  lemma Find_ResultEqual:
    \forall value_type *a, v, integer m, n;
      0 <= Find(a, m, n, v) < n-m  ==>
      a[m + Find(a, m, n, v)] == v;
*/

#endif /* FIND_ACSL_INCLUDED */




/*@
  requires   valid:   \valid_read(a + (0..n-1));

  terminates          \true;
  exits               \false;
  assigns             \nothing;

  ensures    result:  0 <= \result <= n;
  ensures    result:  \result == Find(a, n, v);
*/
size_type find5(const value_type* a, size_type n, value_type v);

#endif /* FIND5_H_INCLUDED */


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



size_type find5(const value_type* a, size_type n, value_type v)
{
  return find2(a, n, v);
}

