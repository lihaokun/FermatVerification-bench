/* ===== include: reverse.c ===== */

/* ===== include: reverse.h ===== */

#ifndef REVERSE_H_INCLUDED
#define REVERSE_H_INCLUDED

/* ===== include: Reverse.acsl ===== */

#ifndef REVERSE_ACSL_INCLUDED
#define REVERSE_ACSL_INCLUDED

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



/*@
  predicate Reverse{K,L}(value_type* a, integer m, integer n, value_type* b, integer q) =
    \forall integer i; m <= i < n  ==>  \at(a[i],K) == \at(b[q-1 - (i-m)],L);

  predicate Reverse{K,L}(value_type* a, integer m, integer n, value_type* b) =
    Reverse{K,L}(a, m, n, b, n);

  predicate Reverse{K,L}(value_type* a, integer n, value_type* b) =
    Reverse{K,L}(a, 0, n, b, n);

  predicate Reverse{K,L}(value_type* a, integer m, integer n, integer q) =
    Reverse{K,L}(a, m, n, a, q);

  predicate Reverse{K,L}(value_type* a, integer m, integer n) =
    Reverse{K,L}(a, m, n, n);

  predicate Reverse{K,L}(value_type* a, integer n) = Reverse{K,L}(a, 0, n);
*/

#endif /* REVERSE_ACSL_INCLUDED */



/*@
  requires   valid:    \valid(a + (0..n-1));

  terminates           \true;
  exits                \false;
  assigns              a[0..n-1];

  ensures    reverse:  Reverse{Old,Here}(a, n);
*/
void reverse(value_type* a, size_type n);

#endif  /* REVERSE_H_INCLUDED */


/* ===== include: Unchanged.acsl ===== */

#ifndef UNCHANGED_ACSL_INCLUDED
#define UNCHANGED_ACSL_INCLUDED


/*@
  predicate Unchanged{K,L}(value_type* a, integer m, integer n) =
    \forall integer i; m <= i < n ==>  \at(a[i],K) == \at(a[i],L);

  predicate Unchanged{K,L}(value_type* a, integer n) = Unchanged{K,L}(a, 0, n);
*/

#endif /* UNCHANGED_ACSL_INCLUDED */


/* ===== include: swap.h ===== */

#ifndef SWAP_H_INCLUDED
#define SWAP_H_INCLUDED


/*@
  requires   valid:     \valid(p);
  requires   valid:     \valid(q);

  terminates            \true;
  exits                 \false;
  assigns               *p, *q;

  ensures    exchange:  *p == \old(*q);
  ensures    exchange:  *q == \old(*p);
*/
void swap(value_type* p, value_type* q);

#endif /* SWAP_H_INCLUDED */



void reverse(value_type* a, size_type n)
{
  const size_type half = n / 2u;

  //@ assert half:    half <= n - half;
  //@ assert half:  2*half <= n <= 2*half + 1;
  /*@
    loop invariant bound:    0 <= i <= half <= n-i;
    loop invariant left:     Reverse{Pre,Here}(a, 0, i, n);
    loop invariant middle:   Unchanged{Pre,Here}(a, i, n-i);
    loop invariant right:    Reverse{Pre,Here}(a, n-i, n, i);
    loop assigns i, a[0..n-1];
    loop variant half - i;
  */
  for (size_type i = 0u; i < half; ++i) {
    swap(&a[i], &a[n - 1u - i]);
  }
}

