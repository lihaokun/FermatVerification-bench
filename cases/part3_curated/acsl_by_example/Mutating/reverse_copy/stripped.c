/* ===== include: reverse_copy.c ===== */

/* ===== include: reverse_copy.h ===== */

#ifndef REVERSE_COPY_H_INCLUDED
#define REVERSE_COPY_H_INCLUDED

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


/* ===== include: Unchanged.acsl ===== */

#ifndef UNCHANGED_ACSL_INCLUDED
#define UNCHANGED_ACSL_INCLUDED




#endif /* UNCHANGED_ACSL_INCLUDED */




void reverse_copy(const value_type* a, size_type n, value_type* b);

#endif /* REVERSE_COPY_H_INCLUDED */


void reverse_copy(const value_type* a, size_type n, value_type* b)
{
  
  for (size_type i = 0u; i < n; ++i) {
    b[i] = a[n - 1u - i];
  }
}

