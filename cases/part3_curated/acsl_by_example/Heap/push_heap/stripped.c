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


/* ===== include: MultisetReorder.acsl ===== */

#ifndef MULTISETUNCHANGED_ACSL_INCLUDED
#define MULTISETUNCHANGED_ACSL_INCLUDED

/* ===== include: Count.acsl ===== */

#ifndef COUNT_ACSL_INCLUDED
#define COUNT_ACSL_INCLUDED

/* ===== include: Equal.acsl ===== */

#ifndef EQUAL_ACSL_INCLUDED
#define EQUAL_ACSL_INCLUDED




#endif /* EQUAL_ACSL_INCLUDED */


/* ===== include: UnchangedLemmas.acsl ===== */

#ifndef UNCHANGEDLEMMAS_ACSL_INCLUDED
#define UNCHANGEDLEMMAS_ACSL_INCLUDED




#endif /* UNCHANGEDLEMMAS_ACSL_INCLUDED */





#endif /* COUNT_ACSL_INCLUDED */





#endif /* MULTISETUNCHANGED_ACSL_INCLUDED */




void push_heap(value_type* a, size_type n);

#endif /* PUSH_HEAP_H_INCLUDED */


/* ===== include: heap_parent.h ===== */

#ifndef HEAP_PARENT_H_INCLUDED
#define HEAP_PARENT_H_INCLUDED



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




#endif /* MULTISETOPERATIONS_ACSL_INCLUDED */





#endif /* MULTISETRETAINREST_ACSL_INCLUDED */





#endif /* MULTISETPARITY_ACSL_INCLUDED */





#endif /* MULTISETUPDATE_ACSL_INCLUDED */



void push_heap(value_type* a, size_type n)
{
  if (1u < n) { // otherwise nothings needs to be done
    size_type c = n - 1u;
    size_type p = heap_parent(c);
    

    if (a[p] < a[c]) {
      const value_type v  = a[c];
      a[c] = a[p];
      
      
      

      
      for (c = p, p = heap_parent(c); 0u < c && a[p] < v;
           c = p, p = heap_parent(c)) {
        
        if (a[c] < a[p]) {
          a[c] = a[p];
          
          
          
          
          
        }
      }

      
      
      
      
      
      a[c] = v;
      
      
      
      
      
      
    }
  }
}

