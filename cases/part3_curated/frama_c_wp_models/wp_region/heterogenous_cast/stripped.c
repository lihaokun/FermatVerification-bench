/* run.config_qualif
   OPT: -wp-model region -wp-timeout 5
*/


void g (int *p) {
  *p = 42;
  short *q = (short*) p;
  q[0] = -1;
  q[1] = -1;
  
}
