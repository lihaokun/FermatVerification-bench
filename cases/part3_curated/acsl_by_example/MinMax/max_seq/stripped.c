/* ===== include: max_seq.c ===== */

/* ===== include: max_seq.h ===== */

#ifndef MAX_SEQ_H_INCLUDED
#define MAX_SEQ_H_INCLUDED

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




value_type max_seq(const value_type* p, size_type n);

#endif /* MAX_SEQ_H_INCLUDED */


/* ===== include: max_element2.h ===== */

#ifndef MAX_ELEMENT2_H_INCLUDED
#define MAX_ELEMENT2_H_INCLUDED

/* ===== include: ArrayExtrema.acsl ===== */

#ifndef ARRAYEXTREMA_ACSL_INCLUDED
#define ARRAYEXTREMA_ACSL_INCLUDED

/* ===== include: ArrayBounds.acsl ===== */

#ifndef ARRAYBOUNDS_ACSL_INCLUDED
#define ARRAYBOUNDS_ACSL_INCLUDED

/* ===== include: LessThanComparable.acsl ===== */

#ifndef LESSTHANCOMPARABLE_ACSL_INCLUDED
#define LESSTHANCOMPARABLE_ACSL_INCLUDED




#endif /* LESSTHANCOMPARABLE_ACSL_INCLUDED */





#endif /* ARRAYBOUNDS_ACSL_INCLUDED */





#endif /* ARRAYEXTREMA_ACSL_INCLUDED */




size_type max_element2(const value_type* a, size_type n);

#endif /* MAX_ELEMENT2_H_INCLUDED */


value_type max_seq(const value_type* p, size_type n)
{
  return p[max_element2(p, n)];
}

