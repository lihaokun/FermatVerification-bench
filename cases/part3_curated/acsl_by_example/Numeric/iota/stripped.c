/* ===== include: iota.c ===== */

/* ===== include: iota.h ===== */

#ifndef IOTA_H_INCLUDED
#define IOTA_H_INCLUDED

/* ===== include: IotaGenerate.acsl ===== */

#ifndef IOTAGENERATE_ACSL_INCLUDED
#define IOTAGENERATE_ACSL_INCLUDED

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





#endif /* IOTAGENERATE_ACSL_INCLUDED */


/* could not resolve: #include "limits.h" */


void iota(value_type* a, size_type n, value_type v);

#endif /* IOTA_H_INCLUDED */



void iota(value_type* a, size_type n, value_type v)
{
  
  for (size_type i = 0u; i < n; ++i) {
    a[i] = v++;
  }
}
