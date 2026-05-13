/* ===== include: stack_pop.c ===== */

/* ===== include: stack_pop.h ===== */

#ifndef STACK_POP_H_INCLUDED
#define STACK_POP_H_INCLUDED

/* ===== include: StackPop.acsl ===== */

#ifndef STACKPOP_ACSL_INCLUDED
#define STACKPOP_ACSL_INCLUDED

/* ===== include: StackObservers.acsl ===== */

#ifndef STACKOBSERVERS_ACSL_INCLUDED
#define STACKOBSERVERS_ACSL_INCLUDED

/* ===== include: StackEqual.acsl ===== */

#ifndef STACKEQUAL_ACSL_INCLUDED
#define STACKEQUAL_ACSL_INCLUDED

/* ===== include: Stack.acsl ===== */

#ifndef STACK_ACSL_INCLUDED
#define STACK_ACSL_INCLUDED

/* ===== include: Stack.h ===== */

#ifndef STACK_H_INCLUDED
#define STACK_H_INCLUDED

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



struct Stack {
  value_type*  data;
  size_type    cap;
  size_type    sz;
};

typedef struct Stack Stack;

#endif /* STACK_H_INCLUDED */



/*@
  logic value_type* StackData{S}(Stack* s)     = s->data;

  logic integer     StackCapacity{S}(Stack* s) = s->cap;

  logic integer     StackSize{S}(Stack* s)     = s->sz;
*/

#endif /* STACK_ACSL_INCLUDED */


/* ===== include: Equal.acsl ===== */

#ifndef EQUAL_ACSL_INCLUDED
#define EQUAL_ACSL_INCLUDED


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
  predicate StackPrefix{S,T}(Stack* s, Stack* t) =
    StackSize{S}(s) <= StackSize{T}(t)  &&
    Equal{S,T}(StackData{S}(s), StackSize{S}(s), StackData{T}(t));

  predicate StackEqual{S,T}(Stack* s, Stack* t) =
    StackSize{S}(s) == StackSize{T}(t) &&
    StackPrefix{S,T}(s, t);

  lemma StackEqual_Reflexive{S} :
    \forall Stack* s; StackEqual{S,S}(s, s);

  lemma StackEqual_Symmetric{S,T} :
    \forall Stack *s, *t;
      StackEqual{S,T}(s, t) ==>  StackEqual{T,S}(t, s);

  lemma StackEqual_Transitive{S,T,U}:
    \forall Stack *s, *t, *u;
      StackEqual{S,T}(s, t) ==> StackEqual{T,U}(t, u) ==> StackEqual{S,U}(s, u);
*/

#endif /* STACKEQUAL_ACSL_INCLUDED */


/* ===== include: StackInvariant.acsl ===== */

#ifndef STACKINVARIANT_ACSL_INCLUDED
#define STACKINVARIANT_ACSL_INCLUDED


/*@
  predicate StackInvariant{L}(Stack* s) =
    0 < StackCapacity(s)                           &&
    0 <= StackSize(s) <= StackCapacity(s)          &&
    \valid(StackData(s) + (0..StackCapacity(s)-1)) &&
    \separated(s, StackData(s) + (0..StackCapacity(s)-1));

  predicate StackValid{L}(Stack* s) = \valid(s) && StackInvariant(s);
*/

#endif /* STACKINVARIANT_ACSL_INCLUDED */



/*@
  logic value_type StackTop{S}(Stack* s)  =  StackData(s)[StackSize(s)-1];

  predicate StackEmpty{S}(Stack* s)       =  StackSize(s) == 0;

  predicate StackFull{S}(Stack* s)        =  StackSize(s) == StackCapacity(s);
*/

#endif /* STACKOBSERVERS_ACSL_INCLUDED */



/*@
  predicate StackPop{S,T}(Stack* s, Stack* t) =
    StackCapacity{S}(s) == StackCapacity{T}(t)  &&
    StackSize{S}(s) - 1 == StackSize{T}(t)      &&
    StackPrefix{T,S}(t, s);
*/

#endif /* STACKPOP_ACSL_INCLUDED */



/*@
  requires  valid:      StackValid(s);
  terminates            \true;
  exits                 \false;
  assigns               s->sz;
  ensures   valid:      StackValid(s);

  behavior  empty:
    assumes             StackEmpty(s);
    assigns             \nothing;
    ensures empty:      StackEmpty(s);
    ensures unchanged:  StackEqual{Old,Here}(s,s);

  behavior  not_empty:
    assumes             !StackEmpty(s);
    assigns             s->sz;
    ensures pop:        StackPop{Old,Here}(s, s);
    ensures not_full:   !StackFull(s);

  complete behaviors;
  disjoint behaviors;
*/
void stack_pop(Stack* s);

#endif /* STACK_POP_H_INCLUDED */


/* ===== include: stack_empty.h ===== */

#ifndef STACK_EMPTY_H_INCLUDED
#define STACK_EMPTY_H_INCLUDED


/*@
  requires   valid:      StackValid(s);
  terminates             \true;
  exits                  \false;
  assigns                \nothing;
  ensures    empty:      \result == true   <==>  StackEmpty(s);
  ensures    not_empty:  \result == false  <==> !StackEmpty(s);
*/
bool stack_empty(const Stack* s);

#endif /* STACK_EMPTY_H_INCLUDED */



void stack_pop(Stack* s)
{
  if (!stack_empty(s)) {
    --s->sz;
  }
}

