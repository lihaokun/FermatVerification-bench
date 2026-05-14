/* run.config
   OPT:
   OPT: -wp-model +ref
*/

/* run.config_qualif
   OPT:
   OPT: -wp-model +ref
*/


void f(int *r) { *r = 1 ; }

// Pre-condition of f holds only when q is used ByRef 

int wrong_without_ref(int * q)
{
  f(q) ;
  return *q ;
}

// Pre-condition of f always holds

int pointer(int * q)
{
  f(q) ;
  return *q ;
}

// Pre-condition of f always holds

int local()
{
  int u ;
  f(&u) ;
  return u ;
}

// Pre-condition of f always holds

int formal(int v)
{
  f(&v) ;
  return v ;
}

int g ;

// Pre-condition of f always holds

int global(void)
{
  f(&g) ;
  return g ;
}
