/* ===== include: heap_child.c ===== */

/* ===== include: heap_child.h ===== */

#ifndef HEAP_CHILD_H_INCLUDED
#define HEAP_CHILD_H_INCLUDED

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




#endif /* LESSTHANCOMPARABLE_ACSL_INCLUDED */





#endif /* ARRAYBOUNDS_ACSL_INCLUDED */





#endif /* ARRAYEXTREMA_ACSL_INCLUDED */


/* ===== include: ArrayUpdate.acsl ===== */

#ifndef ARRAYUPDATE_ACSL_INCLUDED
#define ARRAYUPDATE_ACSL_INCLUDED

/* ===== include: At.acsl ===== */

#ifndef AT_ACSL_INCLUDED
#define AT_ACSL_INCLUDED



#endif /* AT_ACSL_INCLUDED */


/* ===== include: Unchanged.acsl ===== */

#ifndef UNCHANGED_ACSL_INCLUDED
#define UNCHANGED_ACSL_INCLUDED




#endif /* UNCHANGED_ACSL_INCLUDED */





#endif /* ARRAYUPDATE_ACSL_INCLUDED */





#endif /* HEAP_ACSL_INCLUDED */




size_type heap_child(const value_type* a, size_type n, size_type p);

#endif /* HEAP_CHILD_H_INCLUDED */



size_type heap_child(const value_type* a, size_type n, size_type p)
{
  if (p + 1u <= n - p - 1u) {
    const size_type left  = 2u * p + 1u;
    const size_type right = left + 1u;

    if (right < n) {
      // case of two children: select child with maximum value
      return a[right] <= a[left] ? left : right;
    }
    else {
      // at most one child that comes before n-1 can exist
      return left;
    }
  }
  else {
    return n;
  }
}

