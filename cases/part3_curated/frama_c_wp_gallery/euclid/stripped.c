/*
  run.config
  DONTRUN:
 */

/*
  run.config_qualif
  OPT: -wp-rte -wp-smoke-tests -wp-driver %{dep:@PTEST_DIR@/euclid.wp}
*/




#include <limits.h>



int euclid_gcd(int a, int b)
{
  int r;
  
  while( b != 0 ) {
    r = b ;
    b = a % b ;
    a = r ;
  }
  return a < 0 ? -a : a;
}
