/* ===== include: replace_copy.c ===== */

/* ===== include: replace_copy.h ===== */

#ifndef REPLACE_COPY_H_INCLUDED
#define REPLACE_COPY_H_INCLUDED

/* ===== include: Replace.acsl ===== */

#ifndef REPLACE_ACSL_INCLUDED
#define REPLACE_ACSL_INCLUDED

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
  predicate Replace{K,L}(value_type* a, integer n,
               value_type* b, value_type v, value_type w) =
    \forall integer i; 0 <= i < n  ==>
      \let ai = \at(a[i],K);
      \let bi = \at(b[i],L);
      (ai == v  ==>  bi == w) && (ai != v  ==>  bi == ai) ;

  predicate Replace{K,L}(value_type* a, integer n, value_type v, value_type w) =
    Replace{K,L}(a, n, a, v, w);
*/

#endif /* REPLACE_ACSL_INCLUDED */


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
  assigns                b[0..n-1];

  ensures    result:     \result == n;
  ensures    replace:    Replace{Old,Here}(a, n, b, v, w);
  ensures    unchanged:  Unchanged{Old,Here}(a, n);
*/
size_type replace_copy(const value_type* a, size_type n, value_type* b,
                       value_type v, value_type w);

#endif /* REPLACE_COPY_H_INCLUDED */



size_type replace_copy(const value_type* a, size_type n, value_type* b,
                       value_type v, value_type w)
{
  /*@
    loop invariant bounds:    0 <= i <= n;
    loop invariant replace:   Replace{Pre,Here}(a, i, b, v, w);
    loop assigns i, b[0..n-1];
    loop variant n-i;
  */
  for (size_type i = 0u; i < n; ++i) {
    b[i] = (a[i] == v ? w : a[i]);
  }

  return n;
}

