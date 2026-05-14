int sum_digits(int num)
{
  int i = 0;
  int sum = 0;
  while (num > 0) {
    i = num % 10;
    
    
    sum += i;
    num /= 10;
  }
  return sum;
}


