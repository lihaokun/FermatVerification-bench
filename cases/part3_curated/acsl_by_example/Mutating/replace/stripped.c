/* ===== include: replace.c ===== */

/* ===== include: replace.h ===== */

#ifndef REPLACE_COPY_H_INCLUDED
#define REPLACE_COPY_H_INCLUDED

/* ===== include: Replace.acsl ===== */

#ifndef REPLACE_ACSL_INCLUDED
#define REPLACE_ACSL_INCLUDED

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





#endif /* REPLACE_ACSL_INCLUDED */


/* ===== include: Unchanged.acsl ===== */

#ifndef UNCHANGED_ACSL_INCLUDED
#define UNCHANGED_ACSL_INCLUDED




#endif /* UNCHANGED_ACSL_INCLUDED */




void replace(value_type* a, size_type n, value_type v, value_type w);

#endif /* REPLACE_COPY_H_INCLUDED */



void replace(value_type* a, size_type n, value_type v, value_type w)
{
  
  for (size_type i = 0u; i < n; ++i) {
    if (a[i] == v) {
      a[i] = w;
    }
  }
}

