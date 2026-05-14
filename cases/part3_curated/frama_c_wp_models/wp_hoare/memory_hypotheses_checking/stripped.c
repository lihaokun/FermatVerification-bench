/* run.config
   OPT:-wp-model +ref -wp-no-warn-memory-model -wp-check-memory-model -then -print
*/

/* run.config_qualif
   OPT:-wp-model +ref -wp-no-warn-memory-model -wp-check-memory-model
*/

int g ;


int sep(int* p){
  return *p + g;
}


void call_sep_ok(void){
  int l = 42;
  sep(&l);
}


void call_sep_bad_sep(void){
  sep(&g);
}


void call_sep_bad_valid(void){
  int * p ;
  {
    int l ;
    p = &l ;
  }
  sep(p);
}

int *p;

int gptr_sep(void){
  return *p + g;
}


void call_gptr_sep_ok(void){
  int l = 42;
  p = &l ;
  gptr_sep();
}


void call_gptr_sep_bad(void){
  p = &g;
  gptr_sep();
}



void assigns_ptr(int *p){
  *p = g + 42 ;
}


void call_assigns_ptr_ok(void){
  int l = 42;
  assigns_ptr(&l);
}


void call_assigns_ptr_bad(void){
  assigns_ptr(&g);
}


int * add_return_ok(int *p){
  (*p) += g ;
  return p ;
}


void call_add_return_ok(void){
  int l = 0;
  int *p = add_return_ok(&l);
  *p = 0;
}


void call_add_return_bad(void){
  int *p = add_return_ok(&g);
  *p = 0;
}


int * bad_return_formal(int x){
  return &x;
}
