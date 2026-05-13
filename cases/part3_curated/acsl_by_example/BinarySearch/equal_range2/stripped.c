/* ===== include: equal_range2.c ===== */

/* ===== include: equal_range2.h ===== */

#ifndef EQUAL_RANGE_H_INCLUDED
#define EQUAL_RANGE_H_INCLUDED

/* ===== include: SizeTypePairTrivial.acsl ===== */

#ifndef SIZETYPEPAIR_ACSL_INCLUDED
#define SIZETYPEPAIR_ACSL_INCLUDED

// dummy specification file that includes size_type_pair.h

/* could not resolve: #include "size_type_pair.h" */

#endif  /* SIZETYPEPAIR_ACSL_INCLUDED */


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


/* ===== include: AllSomeNot.acsl ===== */

#ifndef ALLSOMENOT_ACSL_INCLUDED
#define ALLSOMENOT_ACSL_INCLUDED




#endif /* ALLSOMENOT_ACSL_INCLUDED */


/* ===== include: ArrayBounds.acsl ===== */

#ifndef ARRAYBOUNDS_ACSL_INCLUDED
#define ARRAYBOUNDS_ACSL_INCLUDED

/* ===== include: LessThanComparable.acsl ===== */

#ifndef LESSTHANCOMPARABLE_ACSL_INCLUDED
#define LESSTHANCOMPARABLE_ACSL_INCLUDED




#endif /* LESSTHANCOMPARABLE_ACSL_INCLUDED */





#endif /* ARRAYBOUNDS_ACSL_INCLUDED */




size_type_pair equal_range2(const value_type* a, size_type n, value_type v);

#endif /* EQUAL_RANGE_H_INCLUDED */


/* ===== include: lower_bound.h ===== */

#ifndef LOWER_H_BOUND_INCLUDED
#define LOWER_H_BOUND_INCLUDED



size_type lower_bound(const value_type* a, size_type n, value_type v);

#endif /* LOWER_H_BOUND_INCLUDED */


/* ===== include: upper_bound.h ===== */

#ifndef UPPER_BOUND_H_INCLUDED
#define UPPER_BOUND_H_INCLUDED



size_type upper_bound(const value_type* a, size_type n, value_type v);

#endif /* UPPER_BOUND_H_INCLUDED */


/* could not resolve: #include "make_pair.h" */
/* ===== include: ArrayBoundsShift.acsl ===== */
#ifndef SHIFTLEMMAS_ACSL_INCLUDED
#define SHIFTLEMMAS_ACSL_INCLUDED




#endif /* SHIFTLEMMAS_ACSL_INCLUDED */

/* ===== include: IncreasingLemmas.acsl ===== */

#ifndef INCREASINGLEMMAS_ACSL_INCLUDED
#define INCREASINGLEMMAS_ACSL_INCLUDED

/* ===== include: Equal.acsl ===== */

#ifndef EQUAL_ACSL_INCLUDED
#define EQUAL_ACSL_INCLUDED




#endif /* EQUAL_ACSL_INCLUDED */





#endif /* INCREASINGLEMMAS_ACSL_INCLUDED */



size_type_pair equal_range2(const value_type* a, size_type n, value_type v)
{
  size_type first  = 0u;
  size_type middle = 0u;
  size_type last   = n;

  
  while (last > first) {
    middle = first + (last - first) / 2u;

    if (a[middle] < v) {
      first = middle + 1u;
    }
    else if (v < a[middle]) {
      last = middle;
    }
    else {
      break;
    }
  }

  if (first < last) {
    
    size_type left = first + lower_bound(a + first, middle - first, v);
    
    
    ++middle;
    
    size_type right = middle + upper_bound(a + middle, last - middle, v);
    
    
    
    return make_pair(left, right);
  }
  else {
    return make_pair(first, first);
  }
}

