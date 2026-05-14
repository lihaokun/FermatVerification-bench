
void swap_separated(int *a, int *b)
{
  int tmp = *a ;
  *a = *b;
  *b = tmp ;
}


void swap_aliased(int *a, int *b)
{
  int tmp = *a ;
  *a = *b;
  *b = tmp ;
}
