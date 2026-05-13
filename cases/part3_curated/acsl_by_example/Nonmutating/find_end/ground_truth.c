/* ===== include: find_end.c ===== */

/* ===== include: find_end.h ===== */

#ifndef FIND_END_H_INCLUDED
#define FIND_END_H_INCLUDED

/* ===== include: HasSubRange.acsl ===== */

#ifndef  HASSUBRANGE_ACSL_INCLUDED
#define  HASSUBRANGE_ACSL_INCLUDED

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
  predicate HasSubRange{L}(value_type* a, integer m, integer n, value_type* b, integer p) =
    \exists integer k; (m <= k <= n-p) && Equal{L,L}(a+k, p, b);

  predicate HasSubRange{L}(value_type* a, integer n, value_type* b, integer p) =
    HasSubRange{L}(a, 0, n, b, p);

  lemma HasSubRange_Sizes:
    \forall value_type *a, *b, integer m, n, p;
      HasSubRange(a, m, n, b, p)  ==>  p <= n-m;
*/

#endif /*  HASSUBRANGE_ACSL_INCLUDED */



/*@
  requires   valid:   \valid_read(a + (0..n-1));
  requires   valid:   \valid_read(b + (0..p-1));

  terminates          \true;
  exits               \false;
  assigns             \nothing;

  ensures    result:  0 <= \result <= n;

  behavior has_match:
    assumes          HasSubRange(a, n, b, p);
    assigns          \nothing;
    ensures bound:   0 <= \result <= n-p;
    ensures result:  Equal{Here,Here}(a + \result, p, b);
    ensures last:    !HasSubRange(a, \result + 1, n, b, p);

  behavior no_match:
    assumes          !HasSubRange(a, n, b, p);
    assigns          \nothing;
    ensures result:  \result == n;

  complete behaviors;
  disjoint behaviors;
*/
size_type find_end(const value_type* a, size_type n,
                   const value_type* b, size_type p);

#endif /* FIND_END_H_INCLUDED */



/* ===== include: equal.h ===== */

#ifndef EQUAL2_H_INCLUDED
#define EQUAL2_H_INCLUDED


/*@
  requires   valid:   \valid_read(a + (0..n-1));
  requires   valid:   \valid_read(b + (0..n-1));

  terminates          \true;
  exits               \false;
  assigns             \nothing;

  ensures    result:  \result <==> Equal{Here,Here}(a, n, b);
*/
bool equal(const value_type* a, size_type n, const value_type* b);

#endif /* EQUAL2_H_INCLUDED */



size_type find_end(const value_type* a, size_type n,
                   const value_type* b, size_type p)
{
  size_type r = n;

  if ((0u < p) && (p <= n)) {
    /*@
      loop invariant bound   :  r <= n - p || r == n;
      loop invariant not_found: r == n ==> !HasSubRange(a, p+i-1, b, p);
      loop invariant found:     r < n  ==> Equal{Here,Here}(a+r, p, b);
      loop invariant last:      r < n  ==> !HasSubRange(a, r+1, i+p-1, b, p);
      loop assigns i, r;
      loop variant n - i;
    */
    for (size_type i = 0u; i <= n - p; ++i) {
      if (equal(a + i, p, b)) {
        r = i;
      }
    }
  }

  return r;
}

