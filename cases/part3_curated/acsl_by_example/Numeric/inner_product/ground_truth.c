/* ===== include: inner_product.c ===== */

/* ===== include: inner_product.h ===== */

#ifndef INNER_PRODUCT_H_INCLUDED
#define INNER_PRODUCT_H_INCLUDED

/* ===== include: InnerProduct.acsl ===== */

#ifndef INNERPRODUCT_ACSL_INCLUDED
#define INNERPRODUCT_ACSL_INCLUDED

/* ===== include: UnchangedLemmas.acsl ===== */

#ifndef UNCHANGEDLEMMAS_ACSL_INCLUDED
#define UNCHANGEDLEMMAS_ACSL_INCLUDED

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
  lemma Unchanged_Shrink{K,L}:
    \forall value_type *a, integer m, n, p, q;
       m <= p <= q <= n         ==>
       Unchanged{K,L}(a, m, n)  ==>
       Unchanged{K,L}(a, p, q);

  lemma Unchanged_Extend{K,L}:
    \forall value_type *a, integer n;
      Unchanged{K,L}(a, n)        ==>
      \at(a[n],K) == \at(a[n],L)  ==>
      Unchanged{K,L}(a, n+1);

  lemma Unchanged_Shift{K,L}:
    \forall value_type *a, integer p, q, r;
      Unchanged{K,L}(a+p, q, r)  ==>  Unchanged{K,L}(a, p+q, p+r);

  lemma Unchanged_Symmetric{K,L}:
    \forall value_type *a, integer m, n;
      Unchanged{K,L}(a, m, n)  ==>
      Unchanged{L,K}(a, m, n);

  lemma Unchanged_Transitive{K,L,M}:
    \forall value_type *a, integer m, n;
      Unchanged{K,L}(a, m, n)  ==>
      Unchanged{L,M}(a, m, n)  ==>
      Unchanged{K,M}(a, m, n);
*/

#endif /* UNCHANGEDLEMMAS_ACSL_INCLUDED */



/*@
  logic integer
  InnerProduct{L}(value_type* a, value_type* b, integer n,
                  value_type init) =
    n <= 0 ? init : InnerProduct(a, b, n-1, init) + (a[n-1] * b[n-1]);

  predicate ProductBounds(value_type* a, value_type* b, integer n) =
    \forall integer i; 0 <= i < n  ==>
      VALUE_TYPE_MIN <= a[i] * b[i] <= VALUE_TYPE_MAX;

  predicate InnerProductBounds(value_type* a, value_type* b, integer n,
                     value_type init) =
    \forall integer i; 0 <= i <= n  ==>
      VALUE_TYPE_MIN <= InnerProduct(a, b, i, init) <= VALUE_TYPE_MAX;

  lemma InnerProduct_Init:
    \forall value_type *a, *b, init, integer n;
       n <= 0  ==>  InnerProduct(a, b, n, init) == init;

  lemma InnerProduct_Next{L}:
    \forall value_type* a, *b, init, integer n;
      0 <= n  ==>
      InnerProduct(a, b, n+1, init) == InnerProduct(a, b, n, init) + a[n] * b[n];

  lemma InnerProduct_Unchanged{K,L}:
    \forall value_type *a, *b, init, integer n;
      Unchanged{K,L}(a, n)  ==>
      Unchanged{K,L}(b, n)  ==>
      InnerProduct{K}(a, b, n, init) == InnerProduct{L}(a, b, n, init);
*/

#endif /* INNERPRODUCT_ACSL_INCLUDED */



/*@
  requires   valid:      \valid_read(a + (0..n-1));
  requires   valid:      \valid_read(b + (0..n-1));
  requires   bounds:     ProductBounds(a, b, n);
  requires   bounds:     InnerProductBounds(a, b, n, init);

  terminates             \true;
  exits                  \false;
  assigns                \nothing;

  ensures    result:     \result == InnerProduct(a, b, n, init);
  ensures    unchanged:  Unchanged{Old,Here}(a, n);
  ensures    unchanged:  Unchanged{Old,Here}(b, n);
*/
value_type inner_product(const value_type* a, const value_type* b, size_type n,
                         value_type init);

#endif /* INNER_PRODUCT_H_INCLUDED */



value_type inner_product(const value_type* a, const value_type* b, size_type n,
                         value_type init)
{
  /*@
     loop invariant index: 0 <= i <= n;
     loop invariant inner: init == InnerProduct(a, b, i, \at(init,Pre));
     loop assigns i, init;
     loop variant n-i;
  */
  for (size_type i = 0u; i < n; ++i) {
    /*@
      assert rte_help: init + a[i] * b[i] ==
                       InnerProduct(a, b, i+1, \at(init,Pre));
    */
    init = init + a[i] * b[i];
  }

  return init;
}

