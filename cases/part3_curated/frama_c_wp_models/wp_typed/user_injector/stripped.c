/* run.config
   OPT: -wp-extern-arrays
 */
/* run.config_qualif
   OPT: -wp-extern-arrays
   OPT: -wp-extern-arrays -wp-model +ref
 */

/* -------------------------------------------------------------------------- */
/* --- Sequences                                                          --- */
/* -------------------------------------------------------------------------- */



int seq = 0 ;

/* -------------------------------------------------------------------------- */
/* --- Injector / Collector                                               --- */
/* -------------------------------------------------------------------------- */

int service_cpt ;
int service_id[] ;
int service_result[] ;


int service(int id) ;


/* -------------------------------------------------------------------------- */
/* --- Job                                                                --- */
/* -------------------------------------------------------------------------- */


int job( int a , int b , int * error )
{
  *error = 0 ;

  int ra = service( a );
  int rb = service( b );
  
  if (!ra) *error = a ;
  else if (!rb) *error = b ;

  return ra && rb ;
}
