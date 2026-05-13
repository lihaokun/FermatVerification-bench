/* ===== include: bubble_sort.c ===== */

/* ===== include: bubble_sort.h ===== */

#ifndef BUBBLESORT_H_INCLUDED
#define BUBBLESORT_H_INCLUDED

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


/* ===== include: Increasing.acsl ===== */

#ifndef INCREASING_ACSL_INCLUDED
#define INCREASING_ACSL_INCLUDED

/* ===== include: WeaklyIncreasing.acsl ===== */

#ifndef WEAKLYINCREASING_ACSL_INCLUDED
#define WEAKLYINCREASING_ACSL_INCLUDED





#endif /* WEAKLYINCREASING_ACSL_INCLUDED */






#endif /* INCREASING_ACSL_INCLUDED */


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

/* ===== include: Unchanged.acsl ===== */

#ifndef UNCHANGED_ACSL_INCLUDED
#define UNCHANGED_ACSL_INCLUDED




#endif /* UNCHANGED_ACSL_INCLUDED */





#endif /* UNCHANGEDLEMMAS_ACSL_INCLUDED */





#endif /* COUNT_ACSL_INCLUDED */





#endif /* MULTISETUNCHANGED_ACSL_INCLUDED */




void bubble_sort(value_type* a, size_type n);

#endif /* BUBBLESORT_H_INCLUDED */

/* could not resolve: #include "swap.h" */
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


/* ===== include: MultisetSwap.acsl ===== */

#ifndef MULTISETSWAP_ACSL_INCLUDED
#define MULTISETSWAP_ACSL_INCLUDED

/* ===== include: ArraySwap.acsl ===== */

#ifndef ARRAYSWAP_ACSL_INCLUDED
#define ARRAYSWAP_ACSL_INCLUDED

/* ===== include: At.acsl ===== */

#ifndef AT_ACSL_INCLUDED
#define AT_ACSL_INCLUDED



#endif /* AT_ACSL_INCLUDED */





#endif /* ARRAYSWAP_ACSL_INCLUDED */





#endif /* MULTISETSWAP_ACSL_INCLUDED */


/* ===== include: IncreasingLemmas.acsl ===== */

#ifndef INCREASINGLEMMAS_ACSL_INCLUDED
#define INCREASINGLEMMAS_ACSL_INCLUDED




#endif /* INCREASINGLEMMAS_ACSL_INCLUDED */



void bubble_sort(value_type* a, size_type n)
{
  if (0 < n) {
    
    for (size_type i = 1u; i < n; ++i) {
      
      for (size_type j = 0u; j < n - i; ++j) {
        if (a[j] > a[j + 1u]) {
          
          
          
          swap(&a[j], &a[j + 1u]);
          
          
          
          
          
          
        }
      }
    }
  }

  
}

