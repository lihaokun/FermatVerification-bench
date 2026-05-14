/* run.config*
   STDOPT: #"-cpp-extra-args=-DLE"
   STDOPT: #"-machdep=ppc_32"
*/

struct X {
  char  c ;
  short s ;
  int   i ;
  char  a[5] ;
} ;

struct Y {
  char c ;
  struct X x ;
} ;


void callee(struct Y* y);


void caller(struct Y const* u, struct Y* y){
  callee(y) ;
}

void loader_test(void){
  struct Y y ;
  unsigned char * ptr = &y ;

  
  for(int i = 0 ; i < sizeof(y); i++){
    ptr[i] = i ;
  }
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  

#ifdef LE
  
  
  
  
  
  
  
  
  
#else
  
  
  
  
  
  
  
  
  
#endif
}
