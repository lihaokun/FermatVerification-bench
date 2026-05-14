/* run.config
   OPT: -wp-model +ref -wp-prop="-ko"
   OPT: -wp-model +ref -wp-prop="ko"
*/
/* run.config_qualif
   OPT: -wp-model +ref -wp-prop="-ko"
   OPT: -wp-model +ref -wp-prop="ko"
 */

/*---------------------------------------------------*/

unsigned rl1 (unsigned x) {
  return (x << 1) | (x >> 31);
}


unsigned rr1 (unsigned x) {
  return (x >> 1) | (x << 31);
}

/*---------------------------------------------------*/

unsigned rln32 (unsigned x, int n) {
  return (x << n) | (x >> (32 - n));
}


unsigned rrn32 (unsigned x, int n) {
  return (x << (32 - n)) | (x >> n);
}

/*---------------------------------------------------*/

unsigned long long rln64 (unsigned long long x, int n) {
  return (x << n) | (x >> (64 - n));
}


unsigned long long rrn64 (unsigned long long x, int n) {
  return (x << (64 - n)) | (x >> n);
}
/*---------------------------------------------------*/
