/* run.config
   OPT:
   OPT: -wp-model +ref
*/
/* run.config_qualif
   OPT:
   OPT: -wp-model +ref
*/

int z ;
int *p ;

void job (int x)
{
  
  {
    int y ;
    int z ;
    p = &y ;
    y = 4 ;
    
    
  }
  
}


int f(int x) { return x; }


int g(int x) { x++; return x; }


int * h(int x) { return &x; }
