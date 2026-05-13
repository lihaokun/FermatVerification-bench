/* ===== include: remove_copy3.c ===== */

/* ===== include: remove_copy3.h ===== */

#ifndef REMOVE_COPY3_H_INCLUDED
#define REMOVE_COPY3_H_INCLUDED

/* ===== include: Remove.acsl ===== */

#ifndef REMOVE_ACSL_INCLUDED
#define REMOVE_ACSL_INCLUDED

/* ===== include: IndexOfNotEqual.acsl ===== */

#ifndef INDEXOFNOTEQAUL_ACSL_INCLUDED
#define INDEXOFNOTEQAUL_ACSL_INCLUDED

/* ===== include: CountFindNotEqual.acsl ===== */

#ifndef COUNTFINDNOTEQUAL_ACSL_INCLUDED
#define COUNTFINDNOTEQUAL_ACSL_INCLUDED

/* ===== include: CountNotEqual.acsl ===== */

#ifndef COUNTNOTEQUAL_ACSL_INCLUDED
#define COUNTNOTEQUAL_ACSL_INCLUDED


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


/* ===== include: Count.acsl ===== */

#ifndef COUNT_ACSL_INCLUDED
#define COUNT_ACSL_INCLUDED

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
  logic integer
  CountNotEqual(value_type* a, integer m, integer n, value_type v) = 
    n <= m ? 0 : CountNotEqual(a, m, n-1, v) + (a[n-1] == v ? 0 : 1);

  logic integer
  CountNotEqual(value_type* a, integer n, value_type v) =
    CountNotEqual(a, 0, n, v);

  lemma CountNotEqual_Empty:
    \forall value_type *a, v, integer m, n;
      n <= m  ==>  CountNotEqual(a, m, n, v) == 0;

  lemma CountNotEqual_Hit:
    \forall value_type *a, v, integer m, n;
      m <= n     ==>
      a[n] != v  ==> 
      CountNotEqual(a, m, n+1, v) == CountNotEqual(a, m, n, v) + 1;

  lemma CountNotEqual_Miss:
    \forall value_type *a, v, integer m, n;
      m <= n     ==>
      a[n] == v  ==>
      CountNotEqual(a, m, n+1, v) == CountNotEqual(a, m, n, v);

  lemma CountNotEqual_Lower:
    \forall value_type *a, v, integer m, n;
      m <= n  ==>  0 <= CountNotEqual(a, m, n, v);

  lemma CountNotEqual_Upper:
    \forall value_type *a, v, integer m, n;
      m <= n  ==>  CountNotEqual(a, m, n, v) <= n-m;

  lemma CountNotEqual_WeaklyIncreasing:
    \forall value_type *a, v, integer m, n;
      m <= n  ==>  CountNotEqual(a, m, n, v) <= CountNotEqual(a, m, n+1, v);

  lemma CountNotEqual_Increasing:
    \forall value_type *a, v, integer k, m, n;
      m <= k <= n  ==>  CountNotEqual(a, m, k, v) <= CountNotEqual(a, m, n, v);

  lemma CountNotEqual_Unchanged{K,L}:
    \forall value_type *a, v, integer m, n;
      Unchanged{K,L}(a, m, n)  ==>
      CountNotEqual{K}(a, m, n, v) == CountNotEqual{L}(a, m, n, v);

  lemma CountNotEqual_Count:
    \forall value_type *a, v, integer m, n;
      m <= n  ==>  CountNotEqual(a, m, n, v) == n - m - Count(a, m, n, v);

  lemma CountNotEqual_Union:
    \forall value_type *a, v, integer k, m, n;
      0 <= k <= m <= n  ==>
      CountNotEqual(a, k, n, v) ==
      CountNotEqual(a, k, m, v) + CountNotEqual(a, m, n, v);
*/

#endif /* COUNTNOTEQUAL_ACSL_INCLUDED */



/*@
  lemma CountNotEqual_AllEqual:
    \forall value_type *a, v, integer m, n;
      0 <= m <= n            ==>
      AllEqual(a, m, n, v)   ==>  CountNotEqual(a, m, n, v) == 0;

  lemma CountNotEqual_SomeNotEqual:
    \forall value_type *a, v, integer m, n;
      0 <= m < n                     ==>
      0 < CountNotEqual(a, m, n, v)  ==>  SomeNotEqual(a, m, n, v);

  lemma CountNotEqual_FindNotEqual:
    \forall value_type *a, v, integer m, n;
      0 <= m < n                     ==>
      0 < CountNotEqual(a, m, n, v)  ==>  FindNotEqual(a, m, n, v) < n-m;

  lemma CountNotEqual_Zero:
    \forall value_type *a, v, integer m, n;
      0 <= m < n   ==>
      CountNotEqual(a, m, m + FindNotEqual(a, m, n, v), v) == 0;

  lemma CountNotEqual_Decrement:
    \forall value_type *a, v, integer m, n;
      0 <= m < n  ==>
      CountNotEqual(a, m + FindNotEqual(a, m, n, v), n, v) ==
      CountNotEqual(a, 0, n, v) - CountNotEqual(a, 0, m, v);
*/

#endif /* COUNTFINDNOTEQUAL_H_INCLUDED */



/*@
  logic integer
  IndexOfNotEqual(value_type* a, integer n, value_type v, integer k) =
    \let count = CountNotEqual(a, n, v);
    \let start = IndexOfNotEqual(a, n, v, k-1) + 1;
    // Negative rank: invalid request
    k < 0 ? -1 :
      // Now we know 0 <= k
      (n <= 0 ? 0 :
        // Now we know 0 < n
        k < count ? start + FindNotEqual(a, start, n, v) : n
      );

  lemma IndexOfNotEqual_Left:
    \forall value_type *a, v, integer n, k;
      k < 0  ==>  IndexOfNotEqual(a, n, v, k) == -1;

  lemma IndexOfNotEqual_Empty:
    \forall value_type *a, v, integer n, k;
      n <= 0 <= k  ==>
      IndexOfNotEqual(a, n, v, k) == 0;

  lemma IndexOfNotEqual_Right:
    \forall value_type *a, v, integer n, k;
      0 <= n                       ==>
      CountNotEqual(a, n, v) <= k  ==>  IndexOfNotEqual(a, n, v, k) == n;

  lemma IndexOfNotEqual_Next:
    \forall value_type *a, v, integer n, k;
      \let x = IndexOfNotEqual(a, n, v, k-1) + 1;
      0 <= n                           ==>
      0 <= k < CountNotEqual(a, n, v)  ==> 
      IndexOfNotEqual(a, n, v, k) == x + FindNotEqual(a, x, n, v);

  lemma IndexOfNotEqual_Lower:
    \forall value_type *a, v, integer n, k;
      0 < n                            ==>
      0 <= k < CountNotEqual(a, n, v)  ==>
      0 <= IndexOfNotEqual(a, n, v, k);

  lemma IndexOfNotEqual_Core:
    \forall value_type *a, v, integer n, k;
      \let R = IndexOfNotEqual(a, n, v, k);
      0 < n                            ==>
      0 <= k < CountNotEqual(a, n, v)  ==>
      (R < n      &&
       a[R] != v  &&
       CountNotEqual(a, R, n, v) == CountNotEqual(a, 0, n, v) - k);

  lemma IndexOfNotEqual_Upper:
    \forall value_type *a, v, integer n, k;
      0 < n                            ==>
      0 <= k < CountNotEqual(a, n, v)  ==>
      IndexOfNotEqual(a, n, v, k) < n;

  lemma IndexOfNotEqual_NotEqual:
    \forall value_type *a, v, integer n, k;
      0 < n  ==>
      0 <= k < CountNotEqual(a, n, v)  ==>
      a[IndexOfNotEqual(a, n, v, k)] != v;

  lemma IndexOfNotEqual_Count:
    \forall value_type *a, v, integer n, k;
      0 < n                            ==>
      0 <= k < CountNotEqual(a, n, v)  ==>
      CountNotEqual(a, IndexOfNotEqual(a, n, v, k), n, v) ==
      CountNotEqual(a, 0, n, v) - k;

  lemma IndexOfNotEqual_StrictlyWeakIncreasing:
    \forall value_type *a, v, integer n, k;
      0 < n                           ==>
      0 < k < CountNotEqual(a, n, v)  ==>
      IndexOfNotEqual(a, n, v, k-1) < IndexOfNotEqual(a, n, v, k);

  lemma IndexOfNotEqual_Segment:
    \forall value_type *a, v, integer n, k;
      0 < n                            ==>
      0 <= k                           ==>
      k + 1 < CountNotEqual(a, n, v)   ==>
        AllEqual(a, IndexOfNotEqual(a, n, v, k) + 1,
                    IndexOfNotEqual(a, n, v, k+1), v);

  lemma IndexOfNotEqual_Unchanged{K,L}:
    \forall value_type *a, v, integer n, k;
      Unchanged{K,L}(a, n)  ==>
      IndexOfNotEqual{K}(a, n, v, k) == IndexOfNotEqual{L}(a, n, v, k);
*/

#endif /* INDEXOFNOTEQAUL_H_INCLUDED */



/*@
  predicate Remove{K,L}(value_type* a, integer n, integer i, value_type* b, value_type v) =
    \forall integer k; 0 <= k < CountNotEqual{K}(a, i, v)  ==>
      \let j = IndexOfNotEqual{K}(a, n, v, k);
      \at(b[k],L) == \at(a[j],K);

  predicate Remove{K,L}(value_type* a, integer n, value_type* b, value_type v) =
    Remove{K,L}(a, n, n, b, v);

  predicate Remove{K,L}(value_type* a, integer n, integer i, value_type v) =
    \forall integer k; 0 <= k < CountNotEqual{K}(a, i, v)  ==>
      \let j = IndexOfNotEqual{K}(a, n, v, k);
      \at(a[k],L) == \at(a[j],K);

  predicate Remove{K,L}(value_type* a, integer n, value_type v) =
    Remove{K,L}(a, n, n, v);
*/

#endif /* REMOVE_ACSL_INCLUDED */


/* ===== include: SomeNone.acsl ===== */

#ifndef SOMENONE_ACSL_INCLUDED
#define SOMENONE_ACSL_INCLUDED


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
  requires   valid:      \valid_read(a + (0..n-1));
  requires   valid:      \valid(b + (0..n-1));
  requires   sep:        \separated(a + (0..n-1), b + (0..n-1));

  terminates             \true;
  exits                  \false;
  assigns                b[0..n-1];

  ensures    size:       \result == CountNotEqual{Old}(a, n, v);
  ensures    bound:      0 <= \result <= n;
  ensures    remove:     Remove{Old,Here}(a, n, b, v);
  ensures    discard:    NoneEqual(b, \result, v);
  ensures    unchanged:  Unchanged{Old,Here}(a, n);
  ensures    unchanged:  Unchanged{Old,Here}(b, \result, n);
*/
size_type remove_copy3(const value_type* a, size_type n, value_type* b,
                       value_type v);

#endif /* REMOVE_COPY3_H_INCLUDED */


/* ===== include: At.acsl ===== */

#ifndef AT_ACSL_INCLUDED
#define AT_ACSL_INCLUDED

/*@
  logic value_type At{L}(value_type* x, integer i) = \at(x[i],L);
*/

#endif /* AT_ACSL_INCLUDED */



size_type remove_copy3(const value_type* a, size_type n, value_type* b,
                       value_type v)
{
  size_type k = 0u;

  /*@
    loop invariant size:       k == CountNotEqual{Pre}(a,i,v);
    loop invariant bound:      0 <= k <= i <= n;
    loop invariant remove:     Remove{Pre,Here}(a, n, i, b, v);
    loop invariant discard:    NoneEqual(b, k, v);
    loop invariant interval:   IndexOfNotEqual{Pre}(a, n, v, k-1) <= i;
    loop invariant interval:   i <= IndexOfNotEqual{Pre}(a, n, v, k);
    loop invariant unchanged:  Unchanged{Pre,Here}(a, n);
    loop invariant unchanged:  Unchanged{Pre,Here}(b, k, n);
    loop assigns   k, i, b[0..n-1];
    loop variant   n-i;
  */
  for (size_type i = 0u; i < n; ++i) {
    if (a[i] != v) {
      b[k++] = a[i];
      //@ assert size:      k == CountNotEqual{Pre}(a, i+1, v);
      //@ assert update:    b[k-1] == At{Pre}(a, i);
      //@ assert interval:  i == IndexOfNotEqual{Pre}(a, n, v, k-1);
      //@ assert remove:    Remove{Pre,Here}(a, n, i, b, v);
      //@ assert remove:    Remove{Pre,Here}(a, n, i+1, b, v);
      //@ assert unchanged: Unchanged{Pre,Here}(a, n);
      //@ assert unchanged: Unchanged{Pre,Here}(b, k, n);
    }
    else {
      //@ assert unchanged: Unchanged{Pre,Here}(a, n);
    }

    //@ assert remove:    Remove{Pre,Here}(a, n, i, b, v);
    //@ assert unchanged: Unchanged{Pre,Here}(a, n);
  }

  return k;
}

