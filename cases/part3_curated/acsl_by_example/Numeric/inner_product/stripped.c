/* ===== include: inner_product.c ===== */

/* ===== include: inner_product.h ===== */

#ifndef INNER_PRODUCT_H_INCLUDED
#define INNER_PRODUCT_H_INCLUDED

/* ===== include: InnerProduct.acsl ===== */

#ifndef INNERPRODUCT_ACSL_INCLUDED
#define INNERPRODUCT_ACSL_INCLUDED

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





#endif /* INNERPRODUCT_ACSL_INCLUDED */




value_type inner_product(const value_type* a, const value_type* b, size_type n,
                         value_type init);

#endif /* INNER_PRODUCT_H_INCLUDED */



value_type inner_product(const value_type* a, const value_type* b, size_type n,
                         value_type init)
{
  
  for (size_type i = 0u; i < n; ++i) {
    
    init = init + a[i] * b[i];
  }

  return init;
}

