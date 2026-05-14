int array_search(int *a, int x, int n)
{
  int __retres;
  int p = 0;
  while (p < n) {
    
    if (*(a + p) == x) {
      __retres = 1;
      goto return_label;
    }
    
    p ++;
  }
  __retres = 0;
  return_label: return __retres;
}


