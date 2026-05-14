int sum_digits(int num)
{
  int i = 0;
  int sum = 0;
  while (num > 0) {
    i = num % 10;
    /*@ assert rte: signed_overflow: -2147483648 ≤ sum + i; */
    /*@ assert rte: signed_overflow: sum + i ≤ 2147483647; */
    sum += i;
    num /= 10;
  }
  return sum;
}


