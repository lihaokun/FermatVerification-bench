/* ===== include: max_element.c ===== */

/* ===== include: max_element.h ===== */

#ifndef MAX_ELEMENT_H_INCLUDED
#define MAX_ELEMENT_H_INCLUDED

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




size_type max_element(const value_type* a, size_type n);

#endif /* MAX_ELEMENT_H_INCLUDED */



size_type max_element(const value_type* a, size_type n)
{
  if (0u < n) {
    size_type max = 0u;

    
    for (size_type i = 1u; i < n; i++) {
      if (a[max] < a[i]) {
        max = i;
      }
    }

    return max;
  }

  return n;
}
