




/* bsearch(t,n,v) search for element v in array t
   between index 0 and n-1
   array t is assumed sorted in increasing order
   returns an index i between 0 and n-1 where t[i] equals v,
   or -1 if no element of t is equal to v
 */


int bsearch(int* t, int n, int v) {
  int l = 0, u = n-1;
  
  while (l <= u ) {
    int m = (l + u) / 2;
    if (t[m] < v) l = m + 1;
    else if (t[m] > v) u = m - 1;
    else return m;
  }
  return -1;
}
