/* ===== include: find5.c ===== */

/* ===== include: find5.h ===== */

#ifndef FIND5_H_INCLUDED
#define FIND5_H_INCLUDED

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





size_type find5(const value_type* a, size_type n, value_type v);

#endif /* FIND5_H_INCLUDED */


/* ===== include: find2.h ===== */

#ifndef FIND2_H_INCLUDED
#define FIND2_H_INCLUDED



size_type find2(const value_type* a, size_type n, value_type v);

#endif /* FIND2_H_INCLUDED */



size_type find5(const value_type* a, size_type n, value_type v)
{
  return find2(a, n, v);
}

