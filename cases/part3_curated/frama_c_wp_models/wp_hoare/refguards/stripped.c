/* run.config
   OPT: -wp-model +ref
*/

/* run.config_qualif
   OPT: -wp-model +ref
*/


void f( int *a, int *b ,int *c ,int * d, int k )
{
  a[k] = b[k] ;
  *d = *c ;
  *c = k ;
}


void g( int *a, int *b, int k )
{
  a[k] = b[k] ;
}

/* No warning ; ensures checks the added hyps */

void h( int *c, int *d, int k )
{
  *d = *c ;
  *c = k ;
}

/* No warning ; ensures checks the added hyps */

void s( int **c, int **d, int k )
{
  **d = **c ;
  **c = k ;
}
