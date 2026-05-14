/* run.config_qualif
   OPT:
   OPT: -wp-model +ref
 */



void swap(int *a,int *b)
{
  int tmp = *a ;
  *a = *b ;
  *b = tmp ;
  return ;
}


void main(int a,int b)
{
  if (a >= b) swap(&a,&b) ;
  
}
