/* ===== include: pop_heap.c ===== */

/* ===== include: pop_heap.h ===== */

#ifndef POP_HEAP_H_INCLUDED
#define POP_HEAP_H_INCLUDED

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




void pop_heap(value_type* a, size_type n);

#endif /* POP_HEAP_H_INCLUDED */


/* ===== include: heap_child.h ===== */

#ifndef HEAP_CHILD_H_INCLUDED
#define HEAP_CHILD_H_INCLUDED



size_type heap_child(const value_type* a, size_type n, size_type p);

#endif /* HEAP_CHILD_H_INCLUDED */


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



void pop_heap(value_type* a, size_type n)
{
  if (1u < n) {
    
    if (a[n - 1u] < a[0u]) { // otherwise a[0] == a[n-1] and nothing to be done
      size_type p = 0u;
      const value_type v = a[n - 1u];
      a[n - 1u] = a[p];
      
      
      
      

      size_type c = heap_child(a, n - 1u, p);

      
      for (; c < n - 1u && v < a[c];  p = c, c = heap_child(a, n - 1u, p)) {
        
        
        if (a[c] < a[p]) {
          a[p] = a[c];
          
          
          
          
          
          
          
        }
      }

      
      
      
      
      a[p] = v;
      
      
      
      
      
    }
  }
}

