void bubble_sort(int *a, int n)
{
  int i;
  int j;
  int temp;
  if (n <= 0) goto return_label;
  
  i = n - 1;
  while (i > 0) {
    j = 0;
    while (j < i) {
      
      
      
      if (*(a + j) > *(a + (j + 1))) {
        
        temp = *(a + j);
        
        
        
        *(a + j) = *(a + (j + 1));
        
        
        *(a + (j + 1)) = temp;
      }
      
      j ++;
    }
    
    i --;
  }
  return_label: return;
}


