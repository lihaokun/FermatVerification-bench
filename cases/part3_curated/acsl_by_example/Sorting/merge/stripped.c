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






#endif /* WEAKLYINCREASING_ACSL_INCLUDED */






#endif /* INCREASING_ACSL_INCLUDED */


/* ===== include: ArrayBounds.acsl ===== */

#ifndef ARRAYBOUNDS_ACSL_INCLUDED
#define ARRAYBOUNDS_ACSL_INCLUDED

/* ===== include: LessThanComparable.acsl ===== */

#ifndef LESSTHANCOMPARABLE_ACSL_INCLUDED
#define LESSTHANCOMPARABLE_ACSL_INCLUDED




#endif /* LESSTHANCOMPARABLE_ACSL_INCLUDED */





#endif /* ARRAYBOUNDS_ACSL_INCLUDED */


/* ===== include: Equal.acsl ===== */

#ifndef EQUAL_ACSL_INCLUDED
#define EQUAL_ACSL_INCLUDED




#endif /* EQUAL_ACSL_INCLUDED */





#endif /* INCREASINGLEMMAS_ACSL_INCLUDED */


/* ===== include: Unchanged.acsl ===== */

#ifndef UNCHANGED_ACSL_INCLUDED
#define UNCHANGED_ACSL_INCLUDED




#endif /* UNCHANGED_ACSL_INCLUDED */





void merge(const value_type* a, size_type m,
           const value_type* b, size_type n, value_type* c);


#endif /* MERGE_H_INCLUDED */


/* could not resolve: #include "copy.h" */
/* ===== include: At.acsl ===== */

#ifndef AT_ACSL_INCLUDED
#define AT_ACSL_INCLUDED



#endif /* AT_ACSL_INCLUDED */


/* ===== include: WeaklyIncreasingLemmas.acsl ===== */

#ifndef WEAKLYINCREASINGLEMMAS_ACSL_INCLUDED
#define WEAKLYINCREASINGLEMMAS_ACSL_INCLUDED

/* ===== include: UnchangedLemmas.acsl ===== */

#ifndef UNCHANGEDLEMMAS_ACSL_INCLUDED
#define UNCHANGEDLEMMAS_ACSL_INCLUDED




#endif /* UNCHANGEDLEMMAS_ACSL_INCLUDED */





#endif /* WEAKLYINCREASINGLEMMAS_ACSL_INCLUDED */



void merge(const value_type* a, size_type m,
           const value_type* b, size_type n, value_type* c)
{
  
  size_type i = 0;
  size_type j = 0;
  size_type x = 0;

  if (0 < m || 0 < n) {
    
    while (i < m && j < n) {
      if (a[i] < b[j]) {
        c[x++] = a[i++];
        
        
      }
      else {
        c[x++] = b[j++];
        
        
      }

      
    }

    
    
    
    
    
    
    

    if (i < m) {
      
      
      

      copy(a + i, m - i, c + x);
      
      
      
      
      
      
      
      
      
    }
    else {
      
      
      
      
      

      copy(b + j, n - j, c + x);
      
      
      
      
      
      
      
      
    }

    
    
    
    
    
    
  }
}

