/* ===== include: rotate.c ===== */

/* ===== include: rotate.h ===== */

#ifndef ROTATE_H_INCLUDED
#define ROTATE_H_INCLUDED

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



/*@
  requires   valid:   \valid(a + (0..n-1));
  requires   bound:   p <= n;

  terminates          \true;
  exits               \false;
  assigns             a[0..n-1];

  ensures    result:  \result == n-p;
  ensures    rotate:  Rotate{Old,Here}(a, p, n);
*/
size_type rotate(value_type* a, size_type p, size_type n);

#endif  /* ROTATE_H_INCLUDED */


/* ===== include: reverse.h ===== */

#ifndef REVERSE_H_INCLUDED
#define REVERSE_H_INCLUDED

/* ===== include: Reverse.acsl ===== */

#ifndef REVERSE_ACSL_INCLUDED
#define REVERSE_ACSL_INCLUDED


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


/* ===== include: ReverseLemmas.acsl ===== */

#ifndef REVERSELEMMAS_ACSL_INCLUDED
#define REVERSELEMMAS_ACSL_INCLUDED



/*@
  lemma Reverse_Shift{K,L}:
    \forall value_type* a, integer n, p;
    0 <= p <= n ==>
    (Reverse{K,L}(a + p, 0, n - p) ==> Reverse{K,L}(a, p, n));
*/

#endif /* REVERSELEMMAS_ACSL_INCLUDED */


size_type rotate(value_type* a, size_type p, size_type n)
{
  // if one subrange is empty, then nothings needs to be done
  if ((0u < p) && (p < n)) {
    reverse(a, p);
    reverse(a + p, n - p);
    //@ assert reverse:  Reverse{Pre,Here}(a, p, n);
    reverse(a, n);
  }

  return n - p;
}

