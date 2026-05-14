struct { int f; int *ptr; } comp[20] ;

int *Q ;


void compound(int k) {
  int m = 1;
  Q = &m ; // alias taken on m
  *comp[k].ptr = 4 ;
  
  
}

// NOTE:
// if we require \valid(comp[k].ptr) the goal is provable without frame conditions
// since it can not be aliased with 'm' at PRE, which is not (yet) valid.

// For those two examples, we want the assert false to fail:
//  -> the frame condition must *not* introduce incoherence on initialization

void local_region(void) {
  char b[4] = {0};
  char *x = b ;
  char **in_memtyped = &x ; // be sure to put x in MemTyped
  
  
}

struct S { char b ; };

void formal_region(struct S s) {
  char *x = &s.b ;
  char **in_memtyped = &x ; // be sure to put x in MemTyped
  
  
}
