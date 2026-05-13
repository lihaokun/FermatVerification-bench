/* run.config
   OPT: -wp-no-print -wp-rte
*/

/* run.config_qualif
   OPT: -then -wp-rte -wp
*/







int find(int * p, int n, int v) {
  int i = 0;
  
  for (; i < n ; i++)
    if (p[i] == v)
      break;
  return i;
}
//----------------------------------------------------------







int * find_ptr(int * p, int * q, int v) {
  
  for (; p < q; p++) {
    if (*p == v)
      
      break;
   }
  return p;
}
//----------------------------------------------------------

int * iter_ptr(int * p, int * q) {
  
  for (; p < q ; p++);
  return p;
}
