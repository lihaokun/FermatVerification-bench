/* run.config
   OPT: -warn-unsigned-overflow -wp-rte -wp-no-print
*/

/* run.config_qualif
   OPT: -warn-unsigned-overflow -wp-rte -wp-timeout 20
*/

typedef unsigned uint32_t ;
typedef unsigned long long uint64_t ;




uint64_t BinaryMultiplication (uint32_t a, uint32_t b) {
  
  uint64_t r=0;
  uint64_t x=a;
  if (b != 0) {
    
    while (1) {
      
      if (b%2) r=r+x;
      b=b/2;
      if (b==0) break;
      x=x*2;
    };
  }
  return r;
}
