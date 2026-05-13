/* ===== include: make_pair.c ===== */

/* ===== include: make_pair.h ===== */

#ifndef MAKE_PAIR_H_INCLUDED
#define MAKE_PAIR_H_INCLUDED

/* ===== include: SizeTypePairTrivial.acsl ===== */

#ifndef SIZETYPEPAIR_ACSL_INCLUDED
#define SIZETYPEPAIR_ACSL_INCLUDED

// dummy specification file that includes size_type_pair.h

/* ===== include: size_type_pair.h ===== */

#ifndef SIZE_TYPE_PAIR_H_INCLUDED
#define SIZE_TYPE_PAIR_H_INCLUDED

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



struct size_type_pair {
  size_type first;
  size_type second;
};

typedef struct size_type_pair size_type_pair;

#endif /* SIZE_TYPE_PAIR_H_INCLUDED */



#endif  /* SIZETYPEPAIR_ACSL_INCLUDED */




size_type_pair make_pair(size_type first, size_type second);

#endif /* MAKE_PAIR_H_INCLUDED */



size_type_pair make_pair(size_type first, size_type second)
{
  size_type_pair pair = {first, second};
  return pair;
}

