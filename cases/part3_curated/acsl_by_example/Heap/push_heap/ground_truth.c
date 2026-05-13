/* ===== include: push_heap.c ===== */

/* ===== include: push_heap.h ===== */

#ifndef PUSH_HEAP_H_INCLUDED
#define PUSH_HEAP_H_INCLUDED

/* ===== include: Heap.acsl ===== */

#ifndef HEAP_ACSL_INCLUDED
#define HEAP_ACSL_INCLUDED

/* ===== include: HeapNodes.acsl ===== */

#ifndef HEAPNODES_ACSL_INCLUDED
#define HEAPNODES_ACSL_INCLUDED

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
  logic integer HeapLeft(integer i) = 2*i + 1;

  logic integer HeapRight(integer i) = 2*i + 2;

  logic integer HeapParent(integer i) = (i-1) / 2;

  lemma HeapParent_Zero{L}: HeapParent(0) == 0;

  lemma Heap_ParentLeft:
    \forall integer p; 0 <= p  ==>  HeapParent(HeapLeft(p)) == p;

  lemma Heap_ParentRight:
    \forall integer p; 0 <= p  ==>  HeapParent(HeapRight(p)) == p;

  lemma Heap_ParentChild:
    \forall integer c, p;
      0 < c  ==>  HeapParent(c) == p  ==>
      (c == HeapLeft(p) || c == HeapRight(p));

  lemma Heap_Childs:
    \forall integer a, b;
      0 < a  ==>  0 < b               ==>
      HeapParent(a) == HeapParent(b)  ==>
      (a == b || a+1 == b || a == b+1);

  lemma Heap_ParentBounds:
    \forall integer c; 0 < c  ==>  0 <= HeapParent(c) < c;

  lemma Heap_ChildBounds:
    \forall integer p; 0 <= p  ==>  p < HeapLeft(p) < HeapRight(p);
*/

#endif /* HEAPNODES_ACSL_INCLUDED */


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


/* ===== include: ArrayUpdate.acsl ===== */

#ifndef ARRAYUPDATE_ACSL_INCLUDED
#define ARRAYUPDATE_ACSL_INCLUDED

/* ===== include: At.acsl ===== */

#ifndef AT_ACSL_INCLUDED
#define AT_ACSL_INCLUDED

/*@
  logic value_type At{L}(value_type* x, integer i) = \at(x[i],L);
*/

#endif /* AT_ACSL_INCLUDED */


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
  predicate ArrayUpdate{K,L}(value_type* a, integer n, integer i, value_type v) =
      0 <= i < n                 &&
      Unchanged{K,L}(a, 0, i)    &&
      Unchanged{K,L}(a, i+1, n)  &&
      At{K}(a, i) != v           &&
      At{L}(a, i) == v;

  lemma ArrayUpdate_Shrink{K,L}:
    \forall value_type *a, v, integer n, i;
      0 <= i < n-1                  ==>
      ArrayUpdate{K,L}(a, n, i, v)  ==>
      ArrayUpdate{K,L}(a, n-1, i, v);

  lemma ArrayUpdate_UpperBound{K,L}:
    \forall value_type *a, v, w, integer n, i;
      ArrayUpdate{K,L}(a, n, i, v)  ==>
      v <= w                        ==> 
      UpperBound{K}(a, n, w)        ==>
      UpperBound{L}(a, n, w);
*/

#endif /* ARRAYUPDATE_ACSL_INCLUDED */



/*@
  predicate Heap{L}(value_type* a, integer n) =
    \forall integer i; 0 < i < n  ==>  a[i] <= a[HeapParent(i)];

  lemma Heap_Maximum{L} :
    \forall value_type* a, integer n;
      0 < n  ==>  Heap(a, n)  ==>  MaxElement(a, n, 0);

 lemma Heap_Shrink{L}:
   \forall value_type *a, integer m, n;
     0 <= m <= n  ==>  Heap(a, n)   ==>  Heap(a, m);

 lemma Heap_Unchanged{K,L}:
   \forall value_type *a, integer n;
     0 <= n  ==>  Unchanged{K,L}(a, n)  ==>  Heap{K}(a, n)  ==>  Heap{L}(a, n);

 predicate
 HeapCompatible{L}(value_type* a, integer n, integer m, value_type v) =
   (0 <= m < n)                                      &&
   (0 <= HeapParent(m)  ==>  v <= a[HeapParent(m)])  &&
   (HeapLeft(m)  <  n   ==>  a[HeapLeft(m)]  <= v)   &&
   (HeapRight(m) <  n   ==>  a[HeapRight(m)] <= v);

  lemma HeapCompatible_Update{K,L}:
    \forall value_type *a, v, integer m, n;
       0 <= m < n                      ==>
       Heap{K}(a, n)                   ==>
       HeapCompatible{K}(a, n, m, v)   ==>
       ArrayUpdate{K,L}(a, n, m, v)    ==>  Heap{L}(a, n);

*/

#endif /* HEAP_ACSL_INCLUDED */


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
   requires   nonempty:  0 < n;
   requires   valid:     \valid(a + (0..n-1));
   requires   heap:      Heap(a, n-1);

   terminates           \true;
   exits                \false;
   assigns              a[0..n-1];

   ensures    heap:     Heap(a, n);
   ensures    reorder:  MultisetReorder{Old,Here}(a, n);
*/
void push_heap(value_type* a, size_type n);

#endif /* PUSH_HEAP_H_INCLUDED */


/* ===== include: heap_parent.h ===== */

#ifndef HEAP_PARENT_H_INCLUDED
#define HEAP_PARENT_H_INCLUDED


/*@
   terminates          \true;
   exits               \false;
   assigns             \nothing;

   ensures    parent:  \result == HeapParent(child);
 */
size_type heap_parent(size_type child);

#endif /* HEAP_PARENT_H_INCLUDED */


/* ===== include: MultisetUpdate.acsl ===== */

#ifndef MULTISETUPDATE_ACSL_INCLUDED
#define MULTISETUPDATE_ACSL_INCLUDED

/* ===== include: MultisetParity.acsl ===== */

#ifndef MULTISETPARITY_ACSL_INCLUDED
#define MULTISETPARITY_ACSL_INCLUDED

/* ===== include: MultisetRetainRest.acsl ===== */

#ifndef MULTISETRETAINREST_ACSL_INCLUDED
#define MULTISETRETAINREST_ACSL_INCLUDED

/* ===== include: MultisetOperations.acsl ===== */

#ifndef MULTISETOPERATIONS_ACSL_INCLUDED
#define MULTISETOPERATIONS_ACSL_INCLUDED


/*@
  predicate MultisetAdd{K,L}(value_type* a, integer n, value_type v) =
    Count{L}(a, 0, n, v) == Count{K}(a, 0, n, v) + 1;

  predicate MultisetMinus{K,L}(value_type* a, integer n, value_type v) =
    MultisetAdd{L,K}(a, n, v);

  predicate MultisetRetain{K,L}(value_type* a, integer n, value_type v) =
    Count{K}(a, 0, n, v) == Count{L}(a, 0, n, v);

  lemma MultisetAdd_Distinct{K,L}:
    \forall value_type *a, v, integer m, n;
      0 <= m < n                       ==> 
      At{K}(a, m) != v                 ==>  At{L}(a, m) == v  ==>
      MultisetReorder{K,L}(a, 0, m)    ==>
      MultisetReorder{K,L}(a, m+1, n)  ==>  MultisetAdd{K,L}(a, n, v);

  lemma MultisetMinus_Distinct{K,L}:
    \forall value_type *a, v, integer m, n;
      0 <= m < n                       ==>
      At{K}(a, m) == v                 ==>  At{L}(a, m) != v  ==>
      MultisetReorder{K,L}(a, 0, m)    ==>
      MultisetReorder{K,L}(a, m+1, n)  ==>  MultisetMinus{K,L}(a, n, v);

  lemma MultisetRetain_Distinct{K,L}:
    \forall value_type *a, v, integer m, n;
      0 <= m < n                       ==>
      At{K}(a, m) != v                 ==>  At{L}(a, m) != v  ==>
      MultisetReorder{K,L}(a, 0, m)    ==>
      MultisetReorder{K,L}(a, m+1, n)  ==>  MultisetRetain{K,L}(a, n, v);
*/

#endif /* MULTISETOPERATIONS_ACSL_INCLUDED */



/*@
  predicate MultisetRetainRest{K,L}(value_type* a, integer n, value_type  v, value_type w) =
    \forall value_type x;
      x != v  ==>  x != w  ==>  MultisetRetain{K,L}(a, n, x);

  lemma Multiset_AddMinusRetain{K,L,M}:
    \forall value_type *a, u, integer n;
      MultisetAdd{K,L}(a, n, u)    ==>
      MultisetMinus{L,M}(a, n, u)  ==>  MultisetRetain{K,M}(a, n, u);

  lemma Multiset_MinusAddRetain{K,L,M}:
    \forall value_type *a, u, integer n;
      MultisetMinus{K,L}(a, n, u) ==>
      MultisetAdd{L,M}(a, n, u)   ==>  MultisetRetain{K,M}(a, n, u);

  lemma Multiset_AddMinusRetainReorder{K,L,M}:
    \forall value_type *a, u, v, integer n;
      u != v                               ==>
      MultisetAdd{K,L}(a, n, u)            ==>
      MultisetMinus{K,L}(a, n, v)          ==>
      MultisetRetainRest{K,L}(a, n, u, v)  ==>
      MultisetAdd{L,M}(a, n, v)            ==>
      MultisetMinus{L,M}(a, n, u)          ==>
      MultisetRetainRest{L,M}(a, n, v, u)  ==>  MultisetReorder{K,M}(a, n);
*/

#endif /* MULTISETRETAINREST_ACSL_INCLUDED */



/*@
  predicate MultisetParity{K,L}(value_type* a, integer n, value_type u, value_type v) =
      MultisetAdd{K,L}(a, n, u)    &&
      MultisetMinus{K,L}(a, n, v)  &&
      MultisetRetainRest{K,L}(a, n, u, v);


  lemma MultisetParity_UnchangedFirst{K,L,M}:
    \forall value_type *a, u, v, integer n;
      u != v                           ==>
      Unchanged{K,L}(a, n)             ==>
      MultisetParity{L,M}(a, n, u, v)  ==>  MultisetParity{K,M}(a, n, u, v);

  lemma MultisetParity_UnchangedSecond{K,L,M}:
    \forall value_type *a, u, v, integer n;
      u != v                           ==>
      MultisetParity{K,L}(a, n, u, v)  ==>
      Unchanged{L,M}(a, n)             ==>  MultisetParity{K,M}(a, n, u, v);

  lemma MultisetParity_MultisetReorder{K,L,M}:
    \forall value_type *a, u, v, integer n;
      u != v                           ==>
      MultisetParity{K,L}(a, n, u, v)  ==>
      MultisetParity{L,M}(a, n, v, u)  ==>  MultisetReorder{K,M}(a, n);

  lemma MultisetParity_Combined{K,L,M}:
    \forall value_type *a, u, v, w, integer n;
      u != v                           ==>
      u != w                           ==>
      v != w                           ==>
      MultisetParity{K,L}(a, n, u, v)  ==>
      MultisetParity{L,M}(a, n, w, u)  ==>  MultisetParity{K,M}(a, n, w, v);
*/

#endif /* MULTISETPARITY_ACSL_INCLUDED */



/*@
  predicate MultisetUpdate{K,L}(value_type* a, integer n, integer i, value_type v) =
    \let u = At{K}(a, i);
    u != v                           &&
    0 <= i < n                       &&
    MultisetReorder{K,L}(a, 0, i)    &&
    MultisetReorder{K,L}(a, i+1, n)  &&
    MultisetParity{K,L}(a, n, v, u);

  lemma ArrayUpdate_MultisetUpdate{K,L}:
    \forall value_type *a, v, integer n, i;
      ArrayUpdate{K,L}(a, n, i, v)  ==>  MultisetUpdate{K,L}(a, n, i, v);
*/

#endif /* MULTISETUPDATE_ACSL_INCLUDED */



void push_heap(value_type* a, size_type n)
{
  if (1u < n) { // otherwise nothings needs to be done
    size_type c = n - 1u;
    size_type p = heap_parent(c);
    //@ assert parent: p == HeapParent(c);

    if (a[p] < a[c]) {
      const value_type v  = a[c];
      a[c] = a[p];
      //@ assert update:   ArrayUpdate{Pre,Here}(a, n, c, a[p]);
      //@ assert heap:     Heap(a, n);
      //@ assert reorder:  MultisetParity{Pre,Here}(a, n, a[p], v);

      /*@
        loop invariant bound:      0 <= c < n-1;
        loop invariant heap:       Heap(a, n);
        loop invariant less:       a[c] < v;
        loop invariant parent:     p == HeapParent(c);
        loop invariant reorder:    MultisetParity{Pre,Here}(a, n, a[c], v);
        loop invariant unchanged:  Unchanged{Pre,Here}(a, c);
        loop assigns               c, p, a[0..n-1];
        loop variant               c;
      */
      for (c = p, p = heap_parent(c); 0u < c && a[p] < v;
           c = p, p = heap_parent(c)) {
        //@ ghost value_type ac =  a[c];
        if (a[c] < a[p]) {
          a[c] = a[p];
          //@ assert update:     ArrayUpdate{LoopCurrent,Here}(a, n, c, a[p]);
          //@ assert update:     MultisetUpdate{LoopCurrent,Here}(a, n, c, a[p]);
          //@ assert bound:      0 <= c < n;
          //@ assert less:       ac < a[c] < v;
          //@ assert reorder:    MultisetParity{Pre,Here}(a, n, a[c], v);
        }
      }

      //@ ghost Epilogue: ;
      //@ assert heap: 0 == c || v <= a[HeapParent(c)];
      //@ ghost  value_type  ac = a[c];
      //@ assert update:     ac == At{Epilogue}(a, c) < v;
      //@ assert reorder:    MultisetParity{Pre,Here}(a, n, ac, v);
      a[c] = v;
      //@ assert update:   ArrayUpdate{Epilogue,Here}(a, n, c, v);
      //@ assert heap:     HeapCompatible(a, n, c, v);
      //@ assert heap:     Heap(a, n);
      //@ assert update:   MultisetUpdate{Epilogue,Here}(a, n, c, v);
      //@ assert reorder:  MultisetParity{Epilogue,Here}(a, n, v, ac);
      //@ assert reorder:  MultisetReorder{Pre,Here}(a, n);
    }
  }
}

