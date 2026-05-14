int array_max(int *a, int n)
{
  int i = 1;
  
  int max = *(a + 0);
  while (i < n) {
    
    if (max < *(a + i)) 
      
      max = *(a + i);
    
    i ++;
  }
  return max;
}


