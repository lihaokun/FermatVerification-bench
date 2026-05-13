/* ===== include: shuffle.c ===== */

/* ===== include: shuffle.h ===== */

#ifndef SHUFFLE_H_INCLUDED
#define SHUFFLE_H_INCLUDED

/* ===== include: MultisetReorder.acsl ===== */

#ifndef MULTISETUNCHANGED_ACSL_INCLUDED
#define MULTISETUNCHANGED_ACSL_INCLUDED

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





#endif /* MULTISETUNCHANGED_ACSL_INCLUDED */




void shuffle(value_type* a, size_type n, unsigned short* seed);

#endif /* SHUFFLE_H_INCLUDED */


/* ===== include: random_number.h ===== */

#ifndef RANDOM_NUMBER_H_INCLUDED
#define RANDOM_NUMBER_H_INCLUDED



size_type random_number(unsigned short* state, size_type n);



void random_init(unsigned short* state);

#endif /* RANDOM_NUMBER_H_INCLUDED */


/* ===== include: swap.h ===== */

#ifndef SWAP_H_INCLUDED
#define SWAP_H_INCLUDED



void swap(value_type* p, value_type* q);

#endif /* SWAP_H_INCLUDED */


/* ===== include: MultisetSwap.acsl ===== */

#ifndef MULTISETSWAP_ACSL_INCLUDED
#define MULTISETSWAP_ACSL_INCLUDED

/* ===== include: ArraySwap.acsl ===== */

#ifndef ARRAYSWAP_ACSL_INCLUDED
#define ARRAYSWAP_ACSL_INCLUDED

/* ===== include: At.acsl ===== */

#ifndef AT_ACSL_INCLUDED
#define AT_ACSL_INCLUDED



#endif /* AT_ACSL_INCLUDED */





#endif /* ARRAYSWAP_ACSL_INCLUDED */





#endif /* MULTISETSWAP_ACSL_INCLUDED */




void shuffle(value_type* a, size_type n, unsigned short* seed)
{
  if (0u < n) {
    
    for (size_type i = 1u; i < n; ++i) {
      size_type k = random_number(seed, i) + 1u;

      
      if (k < i) {
        swap(&a[k], &a[i]);
        
        
        
      }
      else {
        
        
      }

      
    }
  }
}

