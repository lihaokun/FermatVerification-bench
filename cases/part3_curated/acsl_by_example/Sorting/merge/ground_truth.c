/* ===== include: merge.c ===== */

/* ===== include: merge.h ===== */

#ifndef MERGE_H_INCLUDED
#define MERGE_H_INCLUDED

/* ===== include: IncreasingLemmas.acsl ===== */

#ifndef INCREASINGLEMMAS_ACSL_INCLUDED
#define INCREASINGLEMMAS_ACSL_INCLUDED

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
  requires   bound:        m + n <= SIZE_TYPE_MAX;
  requires   valid:        \valid_read(a + (0..m-1));
  requires   valid:        \valid_read(b + (0..n-1));
  requires   valid:        \valid(c + (0..m+n-1));
  requires   sep:          \separated(a + (0..m-1), c + (0..m+n-1));
  requires   sep:          \separated(b + (0..n-1), c + (0..m+n-1));
  requires   increasing:   Increasing(a, m);
  requires   increasing:   Increasing(b, n);

  terminates               \true;
  exits                    \false;
  assigns                  c[0 .. m+n-1];

  ensures    increasing:   Increasing(c, m + n);
  ensures    unchanged:    Unchanged{Old,Here}(a, m);
  ensures    unchanged:    Unchanged{Old,Here}(b, n);
 */
void merge(const value_type* a, size_type m,
           const value_type* b, size_type n, value_type* c);


#endif /* MERGE_H_INCLUDED */


/* could not resolve: #include "copy.h" */
/* ===== include: At.acsl ===== */

#ifndef AT_ACSL_INCLUDED
#define AT_ACSL_INCLUDED

/*@
  logic value_type At{L}(value_type* x, integer i) = \at(x[i],L);
*/

#endif /* AT_ACSL_INCLUDED */


/* ===== include: WeaklyIncreasingLemmas.acsl ===== */

#ifndef WEAKLYINCREASINGLEMMAS_ACSL_INCLUDED
#define WEAKLYINCREASINGLEMMAS_ACSL_INCLUDED

/* ===== include: UnchangedLemmas.acsl ===== */

#ifndef UNCHANGEDLEMMAS_ACSL_INCLUDED
#define UNCHANGEDLEMMAS_ACSL_INCLUDED


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
  lemma WeaklyIncreasing_Shrink{L}:
    \forall value_type *a, integer m, n, p, q;
      m <= p <= q <= n                ==>
      WeaklyIncreasing(a, m, n)       ==>  WeaklyIncreasing(a, p, q);

  lemma WeaklyIncreasing_AddElement{L}:
    \forall value_type *a, integer n;
      1 < n                           ==>
      a[n-2] <= a[n-1]                ==>
      WeaklyIncreasing(a, n-1)        ==>  WeaklyIncreasing(a, n);

  lemma WeaklyIncreasing_Shift{L}:
    \forall value_type *a, integer m, n;
      WeaklyIncreasing(a + m, 0, n)  <==>  WeaklyIncreasing(a, m, n + m);

  lemma WeaklyIncreasing_Equal{K,L}:
    \forall value_type *a, *b, integer m, n;
      Equal{K,L}(a, m, n, b)          ==>
      WeaklyIncreasing{K}(a, m, n)    ==>  WeaklyIncreasing{L}(b, m, n);

  lemma WeaklyIncreasing_Unchanged{K,L}:
    \forall value_type *a, integer m, n;
      Unchanged{K,L}(a, m, n)         ==>
      WeaklyIncreasing{K}(a, m, n)    ==>  WeaklyIncreasing{L}(a, m, n);

  lemma WeaklyIncreasing_Join{L}:
    \forall value_type *a, integer m, n;
      0 < m < n                       ==>
      WeaklyIncreasing(a, m)          ==>
      WeaklyIncreasing(a, m, n)       ==>
      a[m-1] <= a[m]                  ==>  WeaklyIncreasing(a, n);
*/

#endif /* WEAKLYINCREASINGLEMMAS_ACSL_INCLUDED */



void merge(const value_type* a, size_type m,
           const value_type* b, size_type n, value_type* c)
{
  //@ assert increasing:  WeaklyIncreasing(a, 0, m);
  size_type i = 0;
  size_type j = 0;
  size_type x = 0;

  if (0 < m || 0 < n) {
    /*@ loop invariant index:       0 <= i <= m;
        loop invariant index:       0 <= j <= n;
        loop invariant index:       x == i+j;
        loop invariant index:       0 <= x <= m+n-1;
        loop invariant upper:       i < m  ==>  UpperBound(c, 0, x, a[i]);
        loop invariant upper:       j < n  ==>  UpperBound(c, 0, x, b[j]);
        loop invariant increasing:  WeaklyIncreasing(c, x);
        loop assigns i, j, x, c[0 .. m+n-1];
        loop variant (m+n) - (i+j);
     */
    while (i < m && j < n) {
      if (a[i] < b[j]) {
        c[x++] = a[i++];
        //@ assert upper:       i < m  ==>  UpperBound(c, 0, x, a[i]);
        //@ assert increasing:  WeaklyIncreasing(c, 0, x);
      }
      else {
        c[x++] = b[j++];
        //@ assert upper:       j < n  ==>  UpperBound(c, 0, x, b[j]);
        //@ assert increasing:  WeaklyIncreasing(c, 0, x);
      }

      //@ assert increasing:  WeaklyIncreasing(c, 0, x);
    }

    //@ ghost  Epilogue: ;
    //@ assert index:       x == i+j;
    //@ assert index:       i == m  ^^  j == n;
    //@ assert index:       i <  m  ^^  j <  n;
    //@ assert increasing:  WeaklyIncreasing(c, 0, x);
    //@ assert unchanged:   Unchanged{Pre,Here}(a, 0, m);
    //@ assert increasing:  WeaklyIncreasing(a, 0, m);

    if (i < m) {
      //@ assert upper:       0 < x  ==>  c[x-1] <= a[i];
      //@ assert increasing:  WeaklyIncreasing(a, i, m);
      //@ assert increasing:  WeaklyIncreasing(a+i, 0, m-i);

      copy(a + i, m - i, c + x);
      //@ assert equal:       c[x] == At{Epilogue}(a, i);
      //@ assert equal:       a[i] == At{Epilogue}(a, i);
      //@ assert equal:       c[x] == a[i];
      //@ assert equal:       Equal{Epilogue,Here}(a+i, m-i, c+x);
      //@ assert index:       m-i+x == m+n;
      //@ assert unchanged:   Unchanged{Pre,Here}(a, 0, m);
      //@ assert unchanged:   Unchanged{Pre,Here}(b, 0, n);
      //@ assert increasing:  WeaklyIncreasing(c+x, 0, m-i);
      //@ assert increasing:  WeaklyIncreasing(c, x, m+n);
    }
    else {
      //@ assert upper:       0 < x  ==>  c[x-1] <= b[j];
      //@ assert unchanged:   Unchanged{Pre,Here}(a, 0, m);
      //@ assert unchanged:   Unchanged{Pre,Here}(b, 0, n);
      //@ assert increasing:  WeaklyIncreasing(b, 0, n);
      //@ assert increasing:  WeaklyIncreasing(b+j, 0, n-j);

      copy(b + j, n - j, c + x);
      //@ assert equal:       c[x] == At{Epilogue}(b, j);
      //@ assert equal:       b[j] == At{Epilogue}(b, j);
      //@ assert equal:       c[x] == b[j];
      //@ assert equal:       Equal{Epilogue,Here}(b+j, n-j, c+x);
      //@ assert index:       n-j+x == m+n;
      //@ assert unchanged:   Unchanged{Epilogue,Here}(c, 0, x);
      //@ assert increasing:  WeaklyIncreasing(c+x, 0, n-j);
      //@ assert increasing:  WeaklyIncreasing(c, x, m+n);
    }

    //@ assert unchanged:   Unchanged{Epilogue,Here}(c, 0, x);
    //@ assert increasing:  WeaklyIncreasing(c, 0, x);
    //@ assert increasing:  WeaklyIncreasing(c, x, m+n);
    //@ assert increasing:  0 < x  ==>  c[x-1] <= c[x];
    //@ assert increasing:  0 < x  ==>  WeaklyIncreasing(c, x-1, m+n);
    //@ assert increasing:  WeaklyIncreasing(c, 0, m+n);
  }
}

