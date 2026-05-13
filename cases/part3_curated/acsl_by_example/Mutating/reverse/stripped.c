/* ===== include: reverse.c ===== */

/* ===== include: reverse.h ===== */

#ifndef REVERSE_H_INCLUDED
#define REVERSE_H_INCLUDED

/* ===== include: Reverse.acsl ===== */

#ifndef REVERSE_ACSL_INCLUDED
#define REVERSE_ACSL_INCLUDED

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





#endif /* REVERSE_ACSL_INCLUDED */




void reverse(value_type* a, size_type n);

#endif  /* REVERSE_H_INCLUDED */


/* ===== include: Unchanged.acsl ===== */

#ifndef UNCHANGED_ACSL_INCLUDED
#define UNCHANGED_ACSL_INCLUDED




#endif /* UNCHANGED_ACSL_INCLUDED */


/* ===== include: swap.h ===== */

#ifndef SWAP_H_INCLUDED
#define SWAP_H_INCLUDED



void swap(value_type* p, value_type* q);

#endif /* SWAP_H_INCLUDED */



void reverse(value_type* a, size_type n)
{
  const size_type half = n / 2u;

  
  
  
  for (size_type i = 0u; i < half; ++i) {
    swap(&a[i], &a[n - 1u - i]);
  }
}

