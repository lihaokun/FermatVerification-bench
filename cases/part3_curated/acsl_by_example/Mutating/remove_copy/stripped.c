/* ===== include: remove_copy.c ===== */

/* ===== include: remove_copy.h ===== */

#ifndef REMOVE_COPY_H_INCLUDED
#define REMOVE_COPY_H_INCLUDED

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


/* ===== include: SomeNone.acsl ===== */

#ifndef SOMENONE_ACSL_INCLUDED
#define SOMENONE_ACSL_INCLUDED




#endif /* SOMENONE_ACSL_INCLUDED */



size_type remove_copy(const value_type *a, size_type n, value_type *b,
                      value_type v);

#endif /* REMOVE_COPY_H_INCLUDED */



size_type remove_copy(const value_type *a, size_type n, value_type *b,
                      value_type v)
{
  size_type k = 0u;

  
  for (size_type i = 0u; i < n; ++i) {
    if (a[i] != v) {
      b[k++] = a[i];
    }
  }

  return k;
}

