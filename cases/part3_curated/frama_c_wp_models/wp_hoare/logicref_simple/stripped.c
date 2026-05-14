/* run.config
   OPT: -wp-model +ref -wp-msg-key refusage
*/

/* run.config_qualif
   OPT: -wp-model +ref
*/







int fsimple (int *c) {return *c+1;}

int t[10];


int fsimple_array (void) {return t[3]+1;}


int ftwo_star (int **d) {return **d+1;}



void fvpositive (int *b)
{
  // OK for ref model only
  
  *b = 0 ;
}

