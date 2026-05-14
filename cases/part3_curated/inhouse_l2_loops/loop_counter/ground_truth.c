int loop_counter(void)
{
  int i = 0;
  while (i < 30) 
    /*@ assert rte: signed_overflow: i + 1 ≤ 2147483647; */
    i ++;
  return i;
}


