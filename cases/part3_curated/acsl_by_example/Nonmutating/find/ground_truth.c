/* ===== include: find.c ===== */

/* ===== include: find.h ===== */

#ifndef FIND_H_INCLUDED
#define FIND_H_INCLUDED

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
  requires   \valid_read(a + (0..n-1));

  terminates \true;
  exits      \false;
  assigns    \nothing;

  ensures    0 <= \result <= n;

  behavior some:
    assumes  \exists integer i; 0 <= i < n && a[i] == v;
    assigns  \nothing;
    ensures  0 <= \result < n;
    ensures  a[\result] == v;
    ensures  \forall integer i; 0 <= i < \result ==> a[i] != v;

  behavior none:
    assumes  \forall integer i; 0 <= i < n ==> a[i] != v;
    assigns  \nothing;
    ensures  \result == n;

  complete behaviors;
  disjoint behaviors;
*/
size_type find(const value_type* a, size_type n, value_type v);

#endif /* FIND_H_INCLUDED */



size_type find(const value_type* a, size_type n, value_type v)
{
  /*@
    loop invariant 0 <= i <= n;
    loop invariant \forall integer k; 0 <= k < i ==> a[k] != v;
    loop assigns i;
    loop variant n-i;
   */
  for (size_type i = 0u; i < n; i++) {
    if (a[i] == v) {
      return i;
    }
  }

  return n;
}

