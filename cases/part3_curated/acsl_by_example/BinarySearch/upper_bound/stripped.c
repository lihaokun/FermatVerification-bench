/* ===== include: upper_bound.c ===== */

/* ===== include: upper_bound.h ===== */

#ifndef UPPER_BOUND_H_INCLUDED
#define UPPER_BOUND_H_INCLUDED

/* ===== include: Increasing.acsl ===== */

#ifndef INCREASING_ACSL_INCLUDED
#define INCREASING_ACSL_INCLUDED

/* ===== include: WeaklyIncreasing.acsl ===== */

#ifndef WEAKLYINCREASING_ACSL_INCLUDED
#define WEAKLYINCREASING_ACSL_INCLUDED

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






#endif /* WEAKLYINCREASING_ACSL_INCLUDED */






#endif /* INCREASING_ACSL_INCLUDED */


/* ===== include: ArrayBounds.acsl ===== */

#ifndef ARRAYBOUNDS_ACSL_INCLUDED
#define ARRAYBOUNDS_ACSL_INCLUDED

/* ===== include: LessThanComparable.acsl ===== */

#ifndef LESSTHANCOMPARABLE_ACSL_INCLUDED
#define LESSTHANCOMPARABLE_ACSL_INCLUDED




#endif /* LESSTHANCOMPARABLE_ACSL_INCLUDED */





#endif /* ARRAYBOUNDS_ACSL_INCLUDED */




size_type upper_bound(const value_type* a, size_type n, value_type v);

#endif /* UPPER_BOUND_H_INCLUDED */



size_type upper_bound(const value_type* a, size_type n, value_type v)
{
  size_type left  = 0u;
  size_type right = n;

  
  while (left < right) {
    const size_type middle = left + (right - left) / 2u;

    if (a[middle] <= v) {
      left = middle + 1u;
    }
    else {
      right = middle;
    }
  }

  return right;
}

