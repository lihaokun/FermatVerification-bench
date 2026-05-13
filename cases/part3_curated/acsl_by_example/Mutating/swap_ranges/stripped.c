/* ===== include: swap_ranges.c ===== */

/* ===== include: swap_ranges.h ===== */

#ifndef SWAP_RANGES_H_INCLUDED
#define SWAP_RANGES_H_INCLUDED

/* ===== include: Equal.acsl ===== */

#ifndef EQUAL_ACSL_INCLUDED
#define EQUAL_ACSL_INCLUDED

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





#endif /* EQUAL_ACSL_INCLUDED */




void swap_ranges(value_type* a, size_type n, value_type* b);

#endif /* SWAP_RANGES_H_INCLUDED */


/* ===== include: swap.h ===== */

#ifndef SWAP_H_INCLUDED
#define SWAP_H_INCLUDED



void swap(value_type* p, value_type* q);

#endif /* SWAP_H_INCLUDED */


/* ===== include: Unchanged.acsl ===== */

#ifndef UNCHANGED_ACSL_INCLUDED
#define UNCHANGED_ACSL_INCLUDED




#endif /* UNCHANGED_ACSL_INCLUDED */



void swap_ranges(value_type* a, size_type n, value_type* b)
{
  
  for (size_type i = 0u; i < n; ++i) {
    swap(a + i, b + i);
  }
}

