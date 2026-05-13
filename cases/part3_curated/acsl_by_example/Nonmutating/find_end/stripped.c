/* ===== include: find_end.c ===== */

/* ===== include: find_end.h ===== */

#ifndef FIND_END_H_INCLUDED
#define FIND_END_H_INCLUDED

/* ===== include: HasSubRange.acsl ===== */

#ifndef  HASSUBRANGE_ACSL_INCLUDED
#define  HASSUBRANGE_ACSL_INCLUDED

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





#endif /*  HASSUBRANGE_ACSL_INCLUDED */




size_type find_end(const value_type* a, size_type n,
                   const value_type* b, size_type p);

#endif /* FIND_END_H_INCLUDED */



/* ===== include: equal.h ===== */

#ifndef EQUAL2_H_INCLUDED
#define EQUAL2_H_INCLUDED



bool equal(const value_type* a, size_type n, const value_type* b);

#endif /* EQUAL2_H_INCLUDED */



size_type find_end(const value_type* a, size_type n,
                   const value_type* b, size_type p)
{
  size_type r = n;

  if ((0u < p) && (p <= n)) {
    
    for (size_type i = 0u; i <= n - p; ++i) {
      if (equal(a + i, p, b)) {
        r = i;
      }
    }
  }

  return r;
}

