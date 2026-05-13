/* ===== include: count.c ===== */

/* ===== include: count.h ===== */

#ifndef COUNT_H_INCLUDED
#define COUNT_H_INCLUDED

/* ===== include: Count.acsl ===== */

#ifndef COUNT_ACSL_INCLUDED
#define COUNT_ACSL_INCLUDED

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


/* ===== include: UnchangedLemmas.acsl ===== */

#ifndef UNCHANGEDLEMMAS_ACSL_INCLUDED
#define UNCHANGEDLEMMAS_ACSL_INCLUDED

/* ===== include: Unchanged.acsl ===== */

#ifndef UNCHANGED_ACSL_INCLUDED
#define UNCHANGED_ACSL_INCLUDED




#endif /* UNCHANGED_ACSL_INCLUDED */





#endif /* UNCHANGEDLEMMAS_ACSL_INCLUDED */





#endif /* COUNT_ACSL_INCLUDED */




size_type count(const value_type* a, size_type n, value_type v);

#endif /*  COUNT_H_INCLUDED */



size_type count(const value_type* a, size_type n, value_type v)
{
  size_type counted = 0u;

  
  for (size_type i = 0u; i < n; ++i) {
    if (a[i] == v) {
      counted++;
    }
  }

  return counted;
}

