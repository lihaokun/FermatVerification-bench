void reverse(int *a, int n)
{
  int i = 0;
  
  int j = n - 1;
  while (i < n / 2) {
    
    int temp = *(a + i);
    
    
    *(a + i) = *(a + j);
    
    *(a + j) = temp;
    
    i ++;
    
    j --;
  }
  return;
}


