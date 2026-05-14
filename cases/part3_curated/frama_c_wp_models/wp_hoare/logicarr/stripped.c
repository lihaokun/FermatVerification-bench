/* run.config
   OPT:
*/

/* run.config_qualif
   OPT: -wp -wp-par 1
*/



// ARRAY PASSED BY VALUES


// USELESS LABELS and USELESS \AT


int arr[10];


void job(i,j,k) {
  int tmp = arr[i];
  arr[i] = arr[j] ;
  arr[j] = tmp ;
  arr[k] = tmp ;
}
