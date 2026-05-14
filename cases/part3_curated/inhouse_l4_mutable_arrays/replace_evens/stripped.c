void replace_evens(int *a, int n)
{
  int i = 0;
  while (i < n) {
    if (i % 2 == 0) 
      
      *(a + i) = 0;
    
    i ++;
  }
  return;
}


