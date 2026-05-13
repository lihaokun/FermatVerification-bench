/* ===== include: partial_sort.c ===== */

/* ===== include: partial_sort.h ===== */

#ifndef PARTIAL_SORT_H_INCLUDED
#define PARTIAL_SORT_H_INCLUDED

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


/* ===== include: Partition.acsl ===== */

#ifndef PARTITION_ACSL_INCLUDED
#define PARTITION_ACSL_INCLUDED


/*@
  predicate Partition{L}(value_type* a, integer m, integer n) =
    \forall integer i, k; 0 <= i < m <= k < n  ==>  a[i] <= a[k];
*/

#endif /* PARTITION_ACSL_INCLUDED */


/* ===== include: MultisetReorder.acsl ===== */

#ifndef MULTISETUNCHANGED_ACSL_INCLUDED
#define MULTISETUNCHANGED_ACSL_INCLUDED

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
  requires   valid:      \valid(a + (0..n-1));
  requires   split:      0 <= m <= n;

  terminates              \true;
  exits                   \false;
  assigns                 a[0..n-1];

  ensures    reorder:     MultisetReorder{Old,Here}(a, n);
  ensures    partition:   Partition(a, m, n);
  ensures    increasing:  Increasing(a, m);
*/
void partial_sort(value_type* a, size_type m, size_type n);

#endif /* PARTIAL_SORT_H_INCLUDED */



/* could not resolve: #include "make_heap.h" */
/* could not resolve: #include "pop_heap.h" */
/* could not resolve: #include "push_heap.h" */
/* could not resolve: #include "sort_heap.h" */
/* could not resolve: #include "swap.h" */

/* ===== include: PartitionLemmas.acsl ===== */

#ifndef PARTITIONLEMMAS_ACSL_INCLUDED
#define PARTITIONLEMMAS_ACSL_INCLUDED

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


/* ===== include: CountFind.acsl ===== */

#ifndef COUNTFIND_ACSL_INCLUDED
#define COUNTFIND_ACSL_INCLUDED

/* ===== include: Find.acsl ===== */

#ifndef FIND_ACSL_INCLUDED
#define FIND_ACSL_INCLUDED

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
  lemma Count_SomeEqual:
    \forall value_type *a, v, integer m, n;
      0 <= m < n             ==>
      0 < Count(a, m, n, v)  ==>
      SomeEqual(a, m, n, v);
*/

#endif /* COUNTFIND_H_INCLUDED */



/*@
  lemma MultisetReorder_SomeEqual{K,L}:
    \forall value_type *a, integer n, i;
      0 < n                       ==>
      0 <= i < n                  ==>
      MultisetReorder{K,L}(a, n)  ==>
      SomeEqual{K}(a, n, \at(a[i],L));

  lemma MultisetReorder_LowerBound{K,L}:
    \forall value_type* a, integer n, value_type v;
      0 <= n                      ==>
      MultisetReorder{K,L}(a, n)  ==>
      LowerBound{K}(a, n, v)      ==>
      LowerBound{L}(a, n, v);

  lemma MultisetReorder_UpperBound{K,L}:
    \forall value_type* a, integer n, value_type v;
      0 <= n                        ==>
      MultisetReorder{K,L}(a, n)    ==>
      UpperBound{K}(a, n, v)        ==>
      UpperBound{L}(a, n, v);

  lemma MultisetReorder_PartitionLowerBound{K,L}:
    \forall value_type* a, integer m, n;
      0 < m <= n                     ==>
      MultisetReorder{K,L}(a, 0, m)  ==>
      Partition{K}(a, m, n)          ==>
      Unchanged{K,L}(a, m, n)        ==>
      LowerBound{L}(a, m, n, \at(a[0],L));
*/

#endif /* PARTITIONLEMMAS_ACSL_INCLUDED */


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



void partial_sort(value_type* a, size_type m, size_type n)
{
  if (m > 0u) {
    make_heap(a, m);

    //@ assert reorder: Unchanged{Pre,Here}(a, m, n);
    /*@
      loop invariant bound:     m <= i <= n;
      loop invariant heap:      Heap(a, m);
      loop invariant upper:     UpperBound(a, 0, m, a[0]);
      loop invariant lower:     LowerBound(a, m, i, a[0]);
      loop invariant reorder:   MultisetReorder{Pre,Here}(a, i);
      loop invariant unchanged: Unchanged{Pre,Here}(a, i, n);
      loop assigns              i, a[0..n-1];
      loop variant              n-i;
    */
    for (size_type i = m; i < n; ++i) {
      if (a[i] < a[0u]) {
        //@ ghost BeforePop: ;
        pop_heap(a, m);
        //@ assert lower:      a[0] <= a[m-1];
        //@ assert lower:      a[i] <  a[m-1];
        //@ assert lower:      LowerBound(a, m, i, a[m-1]);
        //@ assert upper:      UpperBound(a, 0, m-1, a[0]);
        //@ assert upper:      UpperBound(a, 0, m,   a[m-1]);
        //@ assert partition:  Partition(a, m, i);
        //@ assert unchanged:  Unchanged{BeforePop,Here}(a, m, i);
        //@ assert reorder:    MultisetReorder{BeforePop,Here}(a, m);
        //@ assert reorder:    MultisetReorder{Pre,Here}(a, i);
        //@ assert unchanged:  Unchanged{Pre,Here}(a, i, n);

        //@ ghost BeforeSwap: ;
        //@ assert reorder:    MultisetReorder{Pre,Here}(a, i+1);
        swap(a + m - 1u, a + i);
        //@ assert swapped:    ArraySwap{BeforeSwap,Here}(a, m-1, i, n);
        //@ assert unchanged:  Unchanged{BeforeSwap,Here}(a, m-1);
        //@ assert unchanged:  Unchanged{Pre,Here}(a, i+1, n);
        //@ assert reorder:    MultisetReorder{BeforeSwap,Here}(a, m-1, i+1);
        //@ assert reorder:    MultisetReorder{BeforeSwap,Here}(a, i+1);
        //@ assert reorder:    MultisetReorder{Pre,Here}(a, i+1);
        //@ assert lower:      a[m-1] < a[i];
        //@ assert partition:  Partition(a, m, i+1);
        //@ assert upper:      UpperBound(a, 0, m-1, a[0]);

        //@ ghost BeforePush: ;
        push_heap(a, m);
        //@ assert heap:       Heap(a, m);
        //@ assert unchanged:  Unchanged{BeforePush,Here}(a, m, i+1);
        //@ assert unchanged:  Unchanged{BeforePush,Here}(a, i+1, n);
        //@ assert reorder:    MultisetReorder{Pre,Here}(a, i+1);
        //@ assert upper:      UpperBound(a, 0, m,   a[0]);
        //@ assert lower:      LowerBound(a, m, i+1, a[0]);
      }
    }

    //@ assert partition: Partition(a, m, n);
    //@ ghost BeforeSort: ;
    sort_heap(a, m);
    //@ assert unchanged:  Unchanged{BeforeSort,Here}(a, m, n);
    //@ assert reorder:    MultisetReorder{BeforeSort,Here}(a, m);
    //@ assert reorder:    MultisetReorder{Pre,Here}(a, n);
    //@ assert partition:  Partition(a, m, n);
  }
}

