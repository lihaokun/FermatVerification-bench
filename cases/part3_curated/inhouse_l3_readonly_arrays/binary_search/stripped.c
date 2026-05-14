int binary_search(int *a, int x, int n)
{
  int __retres;
  int p;
  int low = -1;
  int high = n;
  while (1) {
    
    if (! (low + 1 < high)) break;
    
    
    p = (low + high) / 2;
    
    if (*(a + p) == x) {
      __retres = p;
      goto return_label;
    }
    else 
      
      if (*(a + p) < x) low = p; else high = p;
  }
  __retres = -1;
  return_label: return __retres;
}


