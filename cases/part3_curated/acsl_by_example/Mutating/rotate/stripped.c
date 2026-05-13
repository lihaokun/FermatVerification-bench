/* ===== include: rotate.c ===== */

/* ===== include: rotate.h ===== */

#ifndef ROTATE_H_INCLUDED
#define ROTATE_H_INCLUDED

/* ===== include: Rotate.acsl ===== */

#ifndef ROTATE_ACSL_INCLUDED
#define ROTATE_ACSL_INCLUDED

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





#endif /* ROTATE_ACSL_INCLUDED */




size_type rotate(value_type* a, size_type p, size_type n);

#endif  /* ROTATE_H_INCLUDED */


/* ===== include: reverse.h ===== */

#ifndef REVERSE_H_INCLUDED
#define REVERSE_H_INCLUDED

/* ===== include: Reverse.acsl ===== */

#ifndef REVERSE_ACSL_INCLUDED
#define REVERSE_ACSL_INCLUDED




#endif /* REVERSE_ACSL_INCLUDED */




void reverse(value_type* a, size_type n);

#endif  /* REVERSE_H_INCLUDED */


/* ===== include: ReverseLemmas.acsl ===== */

#ifndef REVERSELEMMAS_ACSL_INCLUDED
#define REVERSELEMMAS_ACSL_INCLUDED





#endif /* REVERSELEMMAS_ACSL_INCLUDED */


size_type rotate(value_type* a, size_type p, size_type n)
{
  // if one subrange is empty, then nothings needs to be done
  if ((0u < p) && (p < n)) {
    reverse(a, p);
    reverse(a + p, n - p);
    
    reverse(a, n);
  }

  return n - p;
}

