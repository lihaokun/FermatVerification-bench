/* ===== include: reverse_copy.c ===== */

/* ===== include: reverse_copy.h ===== */

#ifndef REVERSE_COPY_H_INCLUDED
#define REVERSE_COPY_H_INCLUDED

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


/* ===== include: Unchanged.acsl ===== */

#ifndef UNCHANGED_ACSL_INCLUDED
#define UNCHANGED_ACSL_INCLUDED


/*@
  predicate Unchanged{K,L}(value_type* a, integer m, integer n) =
    \forall integer i; m <= i < n ==>  \at(a[i],K) == \at(a[i],L);

  predicate Unchanged{K,L}(value_type* a, integer n) = Unchanged{K,L}(a, 0, n);
*/

#endif /* UNCHANGED_ACSL_INCLUDED */



/*@
  requires   valid:      \valid_read(a + (0..n-1));
  requires   valid:      \valid(b + (0..n-1));
  requires   sep:        \separated(a + (0..n-1), b + (0..n-1));

  terminates             \true;
  exits                  \false;
  assigns                b[0..(n-1)];

  ensures    reverse:    Reverse{Old,Here}(a, n, b);
  ensures    unchanged:  Unchanged{Old,Here}(a, n);
*/
void reverse_copy(const value_type* a, size_type n, value_type* b);

#endif /* REVERSE_COPY_H_INCLUDED */


void reverse_copy(const value_type* a, size_type n, value_type* b)
{
  /*@
    loop invariant bound:   0 <= i <= n;
    loop invariant reverse: Reverse{Here,Pre}(b, 0, i, a, n);
    loop assigns i, b[0..n-1];
    loop variant n-i;
  */
  for (size_type i = 0u; i < n; ++i) {
    b[i] = a[n - 1u - i];
  }
}

