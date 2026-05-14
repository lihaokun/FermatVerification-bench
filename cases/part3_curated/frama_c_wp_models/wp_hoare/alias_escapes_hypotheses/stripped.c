/* run.config
   OPT:
   OPT:-wp-no-warn-memory-model -wp-check-memory-model -then -print
*/

/* run.config_qualif
  DONTRUN:
*/

int a ;


int* f1(void){
  a = 42 ;
  return (void*) 0 ;
}


int* f2(void){
  return &a ;
}


int* f3(int x){
  return &x ;
}


void fp1(int** p){
  a = 42 ;
  *p = (void*) 0 ;
}


void fp2(int** p){
  *p = &a ;
}


void fp3(int** p, int x){
  *p = &x ;
}
