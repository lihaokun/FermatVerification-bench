/* run.config
   OPT: -wp-model +ref
*/

/* run.config_qualif
   OPT: -wp-model +ref
*/

 
int f(int * p);

 
int g (int x){
  x = 4 ; 
  return f(&x);
}



int f2(int *p2, int * q);


int call_f2(int * ptr, int y)
{
  y = 2 ; *ptr =2; 
  return f2(ptr,&y);
}


int *gl; 


int call_global (void)
{ return f(gl);}


/*--------------------------------------------*/




void write (int kb,int * pa)
{ *pa = kb;}
