int check_evens(int *a, int n)
{
  int __retres;
  int p = 0;
  while (p < n) {
    
    if (*(a + p) % 2 != 0) {
      __retres = 0;
      goto return_label;
    }
    
    p ++;
  }
  __retres = 1;
  return_label: return __retres;
}


