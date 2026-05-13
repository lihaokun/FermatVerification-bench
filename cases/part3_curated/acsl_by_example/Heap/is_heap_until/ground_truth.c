/* ===== include: is_heap_until.c ===== */

/* ===== include: is_heap_until.h ===== */

#ifndef IS_HEAP_UNTIL_H_INCLUDED
#define IS_HEAP_UNTIL_H_INCLUDED

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



/*@
   requires   valid:  \valid_read(a + (0..n-1));

   terminates         \true;
   exits              \false;
   assigns            \nothing;

   ensures    bound:  0 <= \result <= n;
   ensures    heap:   Heap(a, \result);
   ensures    last:   \forall integer i; \result < i <= n ==> !Heap(a, i);
*/
size_type is_heap_until(const value_type* a, size_type n);

#endif /* IS_HEAP_UNTIL_H_INCLUDED */



size_type is_heap_until(const value_type* a, size_type n)
{
  size_type parent = 0u;

  /*@
    loop invariant bound:    0 <= parent < child <= n+1;
    loop invariant parent:   parent == HeapParent(child);
    loop invariant heap:     Heap(a, child);
    loop invariant not_heap: a[parent] < a[child] ==>
                             \forall integer i; child < i <= n ==> !Heap(a, i);
    loop assigns child, parent;
    loop variant n - child;
  */
  for (size_type child = 1u; child < n; ++child) {
    if (a[parent] < a[child]) {
      return child;
    }

    if ((child % 2u) == 0u) {
      ++parent;
    }
  }

  return n;
}

