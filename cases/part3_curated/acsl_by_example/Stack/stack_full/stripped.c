/* ===== include: stack_full.c ===== */

/* ===== include: stack_full.h ===== */

#ifndef STACK_FULL_H_INCLUDED
#define STACK_FULL_H_INCLUDED

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





#endif /* STACK_ACSL_INCLUDED */


/* ===== include: Equal.acsl ===== */

#ifndef EQUAL_ACSL_INCLUDED
#define EQUAL_ACSL_INCLUDED




#endif /* EQUAL_ACSL_INCLUDED */





#endif /* STACKEQUAL_ACSL_INCLUDED */


/* ===== include: StackInvariant.acsl ===== */

#ifndef STACKINVARIANT_ACSL_INCLUDED
#define STACKINVARIANT_ACSL_INCLUDED




#endif /* STACKINVARIANT_ACSL_INCLUDED */





#endif /* STACKOBSERVERS_ACSL_INCLUDED */




bool stack_full(const Stack* s);

#endif /* STACK_FULL_H_INCLUDED */


/* ===== include: stack_size.h ===== */

#ifndef STACK_SIZE_H_INCLUDED
#define STACK_SIZE_H_INCLUDED



size_type stack_size(const Stack* s);

#endif /* STACK_SIZE_H_INCLUDED */



bool stack_full(const Stack* s)
{
  return stack_size(s) == s->cap;
}

