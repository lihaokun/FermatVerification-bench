/* ===== include: search_n.c ===== */

/* ===== include: search_n.h ===== */

#ifndef SEARCH_N_H_INCLUDED
#define SEARCH_N_H_INCLUDED

/* ===== include: HasConstantSubRange.acsl ===== */

#ifndef  HASCONSTANTSUBRANGE_ACSL_INCLUDED
#define  HASCONSTANTSUBRANGE_ACSL_INCLUDED

/* ===== include: AllSomeNot.acsl ===== */

#ifndef ALLSOMENOT_ACSL_INCLUDED
#define ALLSOMENOT_ACSL_INCLUDED

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





#endif /* ALLSOMENOT_ACSL_INCLUDED */





#endif /*  HASCONSTANTSUBRANGE_ACSL_INCLUDED */




size_type search_n(const value_type* a, size_type n, value_type v, size_type p);

#endif /* SEARCH_N_H_INCLUDED */



size_type search_n(const value_type* a, size_type n, value_type v, size_type p)
{
  if (0u < p) {
    if (p <= n) {
      size_type start = 0u;

      
      for (size_type i = 0u; i < n; ++i) {
        if (a[i] != v) {
          start = i + 1u;
          
        }
        else {
          
          
          if (p == i + 1u - start) {
            
            
            
            
            return start;
          }
          else {
            
            continue;
          }
        }

        
      }

      
      return n;
    }
    else {
      
      
      return n;
    }
  }
  else {
    
    
    
    return 0u;
  }
}

