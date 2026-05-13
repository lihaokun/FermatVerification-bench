/* ===== include: partial_sum_inv.c ===== */

/* ===== include: partial_sum.h ===== */

#ifndef PARTIAL_SUM_H_INCLUDED
#define PARTIAL_SUM_H_INCLUDED

/* ===== include: PartialSum.acsl ===== */
#ifndef PARTIALSUM_ACSL_INCLUDED
#define PARTIALSUM_ACSL_INCLUDED

/* ===== include: AccumulateDefault.acsl ===== */

#ifndef ACCUMULATEDEFAULT_ACSL_INCLUDED
#define ACCUMULATEDEFAULT_ACSL_INCLUDED

/* ===== include: Accumulate.acsl ===== */

#ifndef ACCUMULATE_ACSL_INCLUDED
#define ACCUMULATE_ACSL_INCLUDED

/* ===== include: UnchangedLemmas.acsl ===== */

#ifndef UNCHANGEDLEMMAS_ACSL_INCLUDED
#define UNCHANGEDLEMMAS_ACSL_INCLUDED

/* ===== include: Unchanged.acsl ===== */

#ifndef UNCHANGED_ACSL_INCLUDED
#define UNCHANGED_ACSL_INCLUDED

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





#endif /* UNCHANGED_ACSL_INCLUDED */





#endif /* UNCHANGEDLEMMAS_ACSL_INCLUDED */


/* could not resolve: #include "limits.h" */



#endif /* ACCUMULATE_ACSL_INCLUDED */





#endif /* ACCUMULATEDEFAULT_ACSL_INCLUDED */





#endif /* PARTIALSUM_ACSL_INCLUDED */




size_type partial_sum(const value_type* a, size_type n, value_type* b);

#endif /* PARTIAL_SUM_H_INCLUDED */


/* ===== include: adjacent_difference.h ===== */

#ifndef ADJACENT_DIFFERENCE_H_INCLUDED
#define ADJACENT_DIFFERENCE_H_INCLUDED

/* ===== include: AdjacentDifference.acsl ===== */

#ifndef ADJACENTDIFFERENCE_ACSL_INCLUDED
#define ADJACENTDIFFERENCE_ACSL_INCLUDED

/* could not resolve: #include "limits.h" */
/* ===== include: Difference.acsl ===== */

#ifndef DIFFERENCE_ACSL_INCLUDED
#define DIFFERENCE_ACSL_INCLUDED




#endif /* DIFFERENCE_ACSL_INCLUDED */





#endif /* ADJACENTDIFFERENCE_ACSL_INCLUDED */




size_type adjacent_difference(const value_type* a, size_type n, value_type* b);

#endif /* ADJACENT_DIFFERENCE_H_INCLUDED */


/* ===== include: DefaultBounds.acsl ===== */

#ifndef DEFAULTBOUNDS_ACSL_INCLUDED
#define DEFAULTBOUNDS_ACSL_INCLUDED

/* could not resolve: #include "limits.h" */



#endif /* DEFAULTBOUNDS_ACSL_INCLUDED */


/* ===== include: NumericInverse.acsl ===== */


#ifndef NUMERICINVERSE_ACSL_INCLUDED
#define NUMERICINVERSE_ACSL_INCLUDED




#endif /* NUMERICINVERSE_ACSL_INCLUDED */




void partial_sum_inv(value_type* a, size_type n, value_type* b)
{
  partial_sum(a, n, b);
  adjacent_difference(b, n, a);
}

