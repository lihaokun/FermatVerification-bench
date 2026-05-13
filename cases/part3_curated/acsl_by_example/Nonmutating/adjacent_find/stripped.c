/* ===== include: adjacent_find.c ===== */
/* ===== include: adjacent_find.h ===== */

#ifndef ADJACENT_FIND_H_INCLUDED
#define ADJACENT_FIND_H_INCLUDED

/* ===== include: HasEqualNeighbors.acsl ===== */

#ifndef HASEQUALNEIGHBORS_ACSL_INCLUDED
#define HASEQUALNEIGHBORS_ACSL_INCLUDED

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





#endif /* HASEQUALNEIGHBORS_ACSL_INCLUDED */




size_type adjacent_find(const value_type* a, size_type n);

#endif /* ADJACENT_FIND_H_INCLUDED */



size_type adjacent_find(const value_type* a, size_type n)
{
  if (1u < n) {
    
    for (size_type i = 0u; i + 1u < n; ++i) {
      if (a[i] == a[i + 1u]) {
        return  i;
      }
    }
  }

  return n;
}

