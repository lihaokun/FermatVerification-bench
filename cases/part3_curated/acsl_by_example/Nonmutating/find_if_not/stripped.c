/* ===== include: find_if_not.c ===== */

/* ===== include: find_if_not.h ===== */

#ifndef FINDIFNOT_H_INCLUDED
#define FINDIFNOT_H_INCLUDED

/* ===== include: FindNotEqual.acsl ===== */

#ifndef FINDNOTEQUAL_ACSL_INCLUDED
#define FINDNOTEQUAL_ACSL_INCLUDED

/* ===== include: AllSomeNot.acsl ===== */

#ifndef ALLSOMENOT_ACSL_INCLUDED
#define ALLSOMENOT_ACSL_INCLUDED

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





#endif /* ALLSOMENOT_ACSL_INCLUDED */


/* ===== include: UnchangedLemmas.acsl ===== */

#ifndef UNCHANGEDLEMMAS_ACSL_INCLUDED
#define UNCHANGEDLEMMAS_ACSL_INCLUDED

/* ===== include: Unchanged.acsl ===== */

#ifndef UNCHANGED_ACSL_INCLUDED
#define UNCHANGED_ACSL_INCLUDED




#endif /* UNCHANGED_ACSL_INCLUDED */





#endif /* UNCHANGEDLEMMAS_ACSL_INCLUDED */





#endif /* FINDNOTEQUAL_ACSL_INCLUDED */




size_type find_if_not(const value_type* a, size_type n, value_type v);

#endif /* FINDIFNOT_H_INCLUDED */




size_type find_if_not(const value_type* a, size_type n, value_type v)
{
  
  for (size_type i = 0u; i < n; i++) {
    if (a[i] != v) {
      
      return i;
    }
  }

  return n;
}

