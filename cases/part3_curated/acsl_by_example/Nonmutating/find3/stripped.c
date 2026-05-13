/* ===== include: find3.c ===== */

/* ===== include: find3.h ===== */

#ifndef FIND3_H_INCLUDED
#define FIND3_H_INCLUDED

/* ===== include: Find.acsl ===== */

#ifndef FIND_ACSL_INCLUDED
#define FIND_ACSL_INCLUDED

/* ===== include: SomeNone.acsl ===== */

#ifndef SOMENONE_ACSL_INCLUDED
#define SOMENONE_ACSL_INCLUDED

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





#endif /* SOMENONE_ACSL_INCLUDED */





#endif /* FIND_ACSL_INCLUDED */





size_type find3(const value_type* a, size_type n, value_type v);

#endif /* FIND3_H_INCLUDED */




size_type find3(const value_type* a, size_type n, value_type v)
{
  
  for (size_type i = 0u; i < n; i++) {
    if (a[i] == v) {
      
      return i;
    }
  }

  return n;
}

