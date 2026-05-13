/* ===== include: shuffle.c ===== */

/* ===== include: shuffle.h ===== */

#ifndef SHUFFLE_H_INCLUDED
#define SHUFFLE_H_INCLUDED

/* ===== include: MultisetReorder.acsl ===== */

#ifndef MULTISETUNCHANGED_ACSL_INCLUDED
#define MULTISETUNCHANGED_ACSL_INCLUDED

/* ===== include: Count.acsl ===== */

#ifndef COUNT_ACSL_INCLUDED
#define COUNT_ACSL_INCLUDED

/* ===== include: Equal.acsl ===== */

#ifndef EQUAL_ACSL_INCLUDED
#define EQUAL_ACSL_INCLUDED

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
  predicate MultisetReorder{K,L}(value_type* a, integer m, integer n) =
    \forall value_type v;
      Count{K}(a, m, n, v) == Count{L}(a, m, n, v);

  predicate MultisetReorder{K,L}(value_type* a, integer n) =
    MultisetReorder{K,L}(a, 0, n);

  lemma Unchanged_MultisetReorder{K,L}:
    \forall value_type *a, integer k, n;
      Unchanged{K,L}(a, k, n)  ==>  MultisetReorder{K,L}(a, k, n);

  lemma MultisetReorder_DisjointUnion{K,L}:
    \forall value_type *a, integer i, k, n;
      0 <= i <= k <= n               ==>
      MultisetReorder{K,L}(a, i, k)  ==>
      MultisetReorder{K,L}(a, k, n)  ==>
      MultisetReorder{K,L}(a, i, n);

  lemma MultisetReorder_Symmetric{K,L}:
    \forall value_type *a, integer m, n;
      MultisetReorder{K,L}(a, m, n)  ==>  MultisetReorder{L,K}(a, m, n);

  lemma MultisetReorder_Transitive{K,L,M}:
    \forall value_type *a, integer m, n;
      MultisetReorder{K,L}(a, m, n)  ==>
      MultisetReorder{L,M}(a, m, n)  ==>
      MultisetReorder{K,M}(a, m, n);
*/

#endif /* MULTISETUNCHANGED_ACSL_INCLUDED */



/*@
  requires   valid:     \valid(a + (0..n-1));
  requires   valid:     \valid(seed + (0..2));
  requires   sep:       \separated(a + (0..n-1), seed + (0..2));

  terminates            \true;
  exits                 \false;
  assigns               a[0..n-1];
  assigns               seed[0..2];

  ensures     reorder:  MultisetReorder{Old,Here}(a,n);
*/
void shuffle(value_type* a, size_type n, unsigned short* seed);

#endif /* SHUFFLE_H_INCLUDED */


/* ===== include: random_number.h ===== */

#ifndef RANDOM_NUMBER_H_INCLUDED
#define RANDOM_NUMBER_H_INCLUDED


/*@
  requires   pos:     0 < n;
  requires   valid:   \valid(state + (0..2));

  terminates          \true;
  exits               \false;
  assigns             state[0..2];

  ensures    result:  0 <= \result < n;
*/
size_type random_number(unsigned short* state, size_type n);


/*@
  requires   valid:  \valid(state + (0..2));

  terminates         \true;
  exits              \false;
  assigns            state[0..2];
*/
void random_init(unsigned short* state);

#endif /* RANDOM_NUMBER_H_INCLUDED */


/* ===== include: swap.h ===== */

#ifndef SWAP_H_INCLUDED
#define SWAP_H_INCLUDED


/*@
  requires   valid:     \valid(p);
  requires   valid:     \valid(q);

  terminates            \true;
  exits                 \false;
  assigns               *p, *q;

  ensures    exchange:  *p == \old(*q);
  ensures    exchange:  *q == \old(*p);
*/
void swap(value_type* p, value_type* q);

#endif /* SWAP_H_INCLUDED */


/* ===== include: MultisetSwap.acsl ===== */

#ifndef MULTISETSWAP_ACSL_INCLUDED
#define MULTISETSWAP_ACSL_INCLUDED

/* ===== include: ArraySwap.acsl ===== */

#ifndef ARRAYSWAP_ACSL_INCLUDED
#define ARRAYSWAP_ACSL_INCLUDED

/* ===== include: At.acsl ===== */

#ifndef AT_ACSL_INCLUDED
#define AT_ACSL_INCLUDED

/*@
  logic value_type At{L}(value_type* x, integer i) = \at(x[i],L);
*/

#endif /* AT_ACSL_INCLUDED */



/*@
  predicate ArraySwap{K,L}(value_type* a, integer i, integer k, integer n) =
    0 <= i < k < n              &&
    At{K}(a, i) == At{L}(a, k)  &&
    At{K}(a, k) == At{L}(a, i)  &&
    Unchanged{K,L}(a, 0,   i)   &&
    Unchanged{K,L}(a, i+1, k)   &&
    Unchanged{K,L}(a, k+1, n);
*/

#endif /* ARRAYSWAP_ACSL_INCLUDED */



/*@
  lemma MultisetSwap_Middle{K,L}:
    \forall value_type* a, integer i, k, n;
      ArraySwap{K,L}(a, i, k, n)  ==>  MultisetReorder{K,L}(a, i, k+1);

  lemma MultisetSwap_FrontMiddle{K,L}:
    \forall value_type* a, integer i, k, n;
      ArraySwap{K,L}(a, i, k, n)  ==>  MultisetReorder{K,L}(a, 0, k+1);
*/

#endif /* MULTISETSWAP_ACSL_INCLUDED */




void shuffle(value_type* a, size_type n, unsigned short* seed)
{
  if (0u < n) {
    /*@
      loop invariant bounds:    1 <= i <= n;
      loop invariant reorder:   MultisetReorder{Pre,Here}(a, 0, i);
      loop invariant unchanged: Unchanged{Pre,Here}(a, i, n);
      loop assigns   i, a[0..n-1], seed[0..2];
      loop variant   n - i;
    */
    for (size_type i = 1u; i < n; ++i) {
      size_type k = random_number(seed, i) + 1u;

      //@ assert less: 0 <= k <= i;
      if (k < i) {
        swap(&a[k], &a[i]);
        //@ assert swapped: ArraySwap{LoopCurrent,Here}(a, k, i, n);
        //@ assert reorder: MultisetReorder{LoopCurrent,Here}(a, i+1);
        //@ assert reorder: MultisetReorder{Pre,Here}(a, i+1);
      }
      else {
        //@ assert unchanged: Unchanged{LoopCurrent,Here}(a, i+1);
        //@ assert reorder:   MultisetReorder{Pre,Here}(a, i+1);
      }

      //@ assert reorder: MultisetReorder{Pre,Here}(a, i+1);
    }
  }
}

