// assigns p[-128..127] ;
void f(int *p,int *v)
{
  char k = *((char *) v ) ;
  
  p[k] = 1 ;
}
