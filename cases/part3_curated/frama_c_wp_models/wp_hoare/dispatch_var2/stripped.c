/* run.config
   OPT: -wp-model +ref -wp-no-let
   OPT: -wp-model +ref -wp-let
*/

/* run.config_qualif
   OPT: -wp-model +ref -wp-no-let
   OPT: -wp-model +ref -wp-let
*/


void reset (int *rp) {*rp = 0;}


void incr (int *ip) {*ip=*ip+1;}



int load (int *lp) {return *lp;} 

int x; 


int call_global (void) 
{
 reset(&x); 
 return (load(&x));
}


int call_param (int y) 
{
 reset(&y); 
 return (load(&y));
}


int call_local(void) 
{
 int z;
 reset(&z); 
 return (load(&z));
}


int call_param_ref (int *q)
{
 reset(q); 
 return (load(q));
}
