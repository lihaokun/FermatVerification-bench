int array_sum(int *a, int n)
{
  int p = 0;
  int sum = 0;
  while (p < n) {
    
    
    
    sum += *(a + p);
    
    p ++;
  }
  return sum;
}


