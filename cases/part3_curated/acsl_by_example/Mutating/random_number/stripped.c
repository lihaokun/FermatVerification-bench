/* ===== include: random_number.c ===== */

/* ===== include: random_number.h ===== */

#ifndef RANDOM_NUMBER_H_INCLUDED
#define RANDOM_NUMBER_H_INCLUDED

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




size_type random_number(unsigned short* state, size_type n);



void random_init(unsigned short* state);

#endif /* RANDOM_NUMBER_H_INCLUDED */


/* ===== include: C_Bit.acsl ===== */

#ifndef RANDOMNUMBERMODULO_ACSL_INCLUDED
#define RANDOMNUMBERMODULO_ACSL_INCLUDED



#endif /* RANDOMNUMBERMODULO_ACSL_INCLUDED */



// see IEEE 1003.1-2008, 2016 Edition for specification

static long my_lrand48(unsigned short* seed)
{
  unsigned long long state = (unsigned long long)seed[0] << 32
                             | (unsigned long long)seed[1] << 16
                             | (unsigned long long)seed[2];
  state = (0x5deece66dull * state + 0xbull) % (1ull << 48);
  
  long result = state / (1ull << 17);
  
  seed[0u] = state >> 32 & 0xffff;
  seed[1u] = state >> 16 & 0xffff;
  seed[2u] = state >>  8 & 0xffff;
  return result;
}

size_type random_number(unsigned short* state, size_type n)
{
  return my_lrand48(state) % n;
}

void random_init(unsigned short* state)
{
  state[0] = 0x243f;
  state[1] = 0x6a88;
  state[2] = 0x85a3;
}
