/* run.config
   DONTRUN:
*/

/* run.config_qualif
   OPT: -wp-rte -wp-smoke-tests
 */



int binary_search(int * t, int size, int key)
{
  int lo, hi, mid;
  lo = 0; hi = size - 1;
  
  while (lo <= hi) {
    mid = lo + (hi - lo) / 2;
    if (key == t[mid]) return mid;
    if (key < t[mid]) hi = mid - 1; else lo = mid + 1;
  }
  return -1;
}
