/* ===== include: adjacent_difference.c ===== */

/* ===== include: adjacent_difference.h ===== */

#ifndef ADJACENT_DIFFERENCE_H_INCLUDED
#define ADJACENT_DIFFERENCE_H_INCLUDED

/* ===== include: AdjacentDifference.acsl ===== */

#ifndef ADJACENTDIFFERENCE_ACSL_INCLUDED
#define ADJACENTDIFFERENCE_ACSL_INCLUDED

/* could not resolve: #include "limits.h" */
/* ===== include: Difference.acsl ===== */

#ifndef DIFFERENCE_ACSL_INCLUDED
#define DIFFERENCE_ACSL_INCLUDED

/* ===== include: Unchanged.acsl ===== */

#ifndef UNCHANGED_ACSL_INCLUDED
#define UNCHANGED_ACSL_INCLUDED

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





#endif /* UNCHANGED_ACSL_INCLUDED */





#endif /* DIFFERENCE_ACSL_INCLUDED */





#endif /* ADJACENTDIFFERENCE_ACSL_INCLUDED */




size_type adjacent_difference(const value_type* a, size_type n, value_type* b);

#endif /* ADJACENT_DIFFERENCE_H_INCLUDED */



size_type adjacent_difference(const value_type* a, size_type n, value_type* b)
{
  if (0u < n) {
    b[0u] = a[0u];

    
    for (size_type i = 1u; i < n; ++i) {
      
      b[i] = a[i] - a[i - 1u];
      
    }
  }

  return n;
}

