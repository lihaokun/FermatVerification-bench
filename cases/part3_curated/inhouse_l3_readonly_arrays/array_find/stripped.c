int array_find(int *a, int n, int x)
{
  int __retres;
  int i = 0;
  i = 0;
  while (i < n) {
    
    if (*(a + i) == x) {
      __retres = i;
      goto return_label;
    }
    
    i ++;
  }
  __retres = -1;
  return_label: return __retres;
}


