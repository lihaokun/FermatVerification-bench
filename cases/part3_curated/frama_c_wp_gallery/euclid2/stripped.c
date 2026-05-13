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
