/* ===== include: rotate_copy.c ===== */

/* ===== include: rotate_copy.h ===== */

#ifndef ROTATE_COPY_H_INCLUDED
#define ROTATE_COPY_H_INCLUDED

/* ===== include: Rotate.acsl ===== */

#ifndef ROTATE_ACSL_INCLUDED
#define ROTATE_ACSL_INCLUDED

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



/*@
  predicate Equal{K,L}(value_type* a, integer m, integer n, value_type* b) =
    \forall integer i; m <= i < n  ==>  \at(a[i],K) == \at(b[i],L);

  predicate Equal{K,L}(value_type* a, integer n, value_type* b) =
    Equal{K,L}(a, 0, n, b);

  predicate Equal{K,L}(value_type* a, integer m, integer n,
                       value_type* b, integer p) =
    \forall integer k; 0 <= k < n-m ==> \at(a[m+k],K) == \at(b[p+k],L);

  predicate Equal{K,L}(value_type* a, integer m, integer n, integer p) =
      Equal{K,L}(a, m, n, a, p);
*/

#endif /* EQUAL_ACSL_INCLUDED */



/*@
  predicate Rotate{K,L}(value_type* a, integer p, integer n, value_type* b) =
     Equal{K,L}(a, 0, p, b, n-p) &&
     Equal{K,L}(a, p, n, b, 0);

  predicate Rotate{K,L}(value_type* a, integer m, integer p, integer n) =
    Equal{K,L}(a, m, p, m+n-p) &&
    Equal{K,L}(a, p, n, m);

  predicate Rotate{K,L}(value_type* a, integer p, integer n) =
    Rotate{K,L}(a, 0, p, n);
*/

#endif /* ROTATE_ACSL_INCLUDED */


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
  requires   bound:      0 <= p <= n;
  requires   valid:      \valid_read(a + (0..n-1));
  requires   valid:      \valid(b + (0..n-1));
  requires   sep:        \separated(a + (0..n-1), b + (0..n-1));

  terminates             \true;
  exits                  \false;
  assigns                b[0..(n-1)];

  ensures    rotate:     Rotate{Old,Here}(a, p, n, b);
  ensures    unchanged:  Unchanged{Old,Here}(a, n);
*/
void rotate_copy(const value_type* a, size_type p, size_type n, value_type* b);

#endif /* ROTATE_COPY_H_INCLUDED */


/* ===== include: copy.h ===== */

#ifndef COPY_H_INCLUDED
#define COPY_H_INCLUDED


/*@
  requires   valid:  \valid_read(a + (0..n-1));
  requires   valid:  \valid(b + (0..n-1));
  requires   sep:    \separated(a + (0..n-1), b);

  terminates         \true;
  exits              \false;
  assigns            b[0..n-1];

  ensures    equal:  Equal{Old,Here}(a, n, b);
*/
void copy(const value_type* a, const size_type n, value_type* b);

#endif /* COPY_H_INCLUDED */



void rotate_copy(const value_type* a, size_type p, size_type n, value_type* b)
{
  copy(a,  p, b + (n - p));
  copy(a + p, n - p, b);
}

