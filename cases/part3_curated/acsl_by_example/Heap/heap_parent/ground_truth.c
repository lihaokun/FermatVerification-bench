/* ===== include: heap_parent.c ===== */

/* ===== include: heap_parent.h ===== */

#ifndef HEAP_PARENT_H_INCLUDED
#define HEAP_PARENT_H_INCLUDED

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



/*@
   terminates          \true;
   exits               \false;
   assigns             \nothing;

   ensures    parent:  \result == HeapParent(child);
 */
size_type heap_parent(size_type child);

#endif /* HEAP_PARENT_H_INCLUDED */



size_type heap_parent(size_type child)
{
  return (0u < child) ?  (child - 1u) / 2u : 0u;
}

