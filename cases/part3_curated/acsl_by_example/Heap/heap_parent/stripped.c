/* ===== include: heap_parent.c ===== */

/* ===== include: heap_parent.h ===== */

#ifndef HEAP_PARENT_H_INCLUDED
#define HEAP_PARENT_H_INCLUDED

/* ===== include: HeapNodes.acsl ===== */

#ifndef HEAPNODES_ACSL_INCLUDED
#define HEAPNODES_ACSL_INCLUDED

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





#endif /* HEAPNODES_ACSL_INCLUDED */




size_type heap_parent(size_type child);

#endif /* HEAP_PARENT_H_INCLUDED */



size_type heap_parent(size_type child)
{
  return (0u < child) ?  (child - 1u) / 2u : 0u;
}

