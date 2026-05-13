/* ===== include: rewrite_array_nothing.c ===== */

/* ===== include: rewrite_array_nothing.h ===== */

#ifndef REWRITE_ARRAY_H_INCLUDED
#define REWRITE_ARRAY_H_INCLUDED

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



/*@
  predicate Unchanged{K,L}(value_type* a, integer m, integer n) =
    \forall integer i; m <= i < n ==>  \at(a[i],K) == \at(a[i],L);

  predicate Unchanged{K,L}(value_type* a, integer n) = Unchanged{K,L}(a, 0, n);
*/

#endif /* UNCHANGED_ACSL_INCLUDED */



/*@
  requires \valid(a + (0..n-1));
  assigns  \nothing;
  ensures  Unchanged{Pre,Post}(a,0,n);
*/
void rewrite_array(value_type* a, size_type n);

#endif /* REWRITE_ARRAY_H_INCLUDED */



void rewrite_array(value_type* a, size_type n)
{
  /*@
    loop invariant 0u <= i <= n;
    loop invariant array_prefix: Unchanged{Pre,Here}(a, 0, i);
    loop invariant array_suffix: Unchanged{Pre,Here}(a, i, n);
    loop assigns i, a[0..n-1];
    loop variant n - i;
  */
  for (size_type i = 0u; i < n; i++) {
    a[i] = a[i];
  }
}

