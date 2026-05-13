/* ===== include: rotate_copy.c ===== */

/* ===== include: rotate_copy.h ===== */

#ifndef ROTATE_COPY_H_INCLUDED
#define ROTATE_COPY_H_INCLUDED

/* ===== include: Rotate.acsl ===== */

#ifndef ROTATE_ACSL_INCLUDED
#define ROTATE_ACSL_INCLUDED

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





#endif /* ROTATE_ACSL_INCLUDED */


/* ===== include: Unchanged.acsl ===== */

#ifndef UNCHANGED_ACSL_INCLUDED
#define UNCHANGED_ACSL_INCLUDED




#endif /* UNCHANGED_ACSL_INCLUDED */




void rotate_copy(const value_type* a, size_type p, size_type n, value_type* b);

#endif /* ROTATE_COPY_H_INCLUDED */


/* ===== include: copy.h ===== */

#ifndef COPY_H_INCLUDED
#define COPY_H_INCLUDED



void copy(const value_type* a, const size_type n, value_type* b);

#endif /* COPY_H_INCLUDED */



void rotate_copy(const value_type* a, size_type p, size_type n, value_type* b)
{
  copy(a,  p, b + (n - p));
  copy(a + p, n - p, b);
}

