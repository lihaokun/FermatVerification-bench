/* run.config_qualif
   OPT: -wp-prop=-lack
 */





int F1(int n)
{
  if (n<=1) return 1;
  int p=1,i=2 ;
  
  while (i <= n) { p *= i ; i++; }
  return p;
}


int F2(int n)
{
  int p=1,i=2 ;
  
  while (i <= n) { p *= i ; i++; }
  return p;
}


int F4(int n)
{
  int p=1 ;
  
  while (n > 1) { 
    p *= n ; n--; 
  }
  return p;
}
