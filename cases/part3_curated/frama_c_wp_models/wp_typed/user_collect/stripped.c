/* run.config
   STDOPT: +"-wp-no-extensional"
*/
/* run.config_qualif
   STDOPT: +"-wp-no-extensional"
*/

int k ;
int p[10] ;

struct S { int f[2] ; } ;
struct S q[10] ;


void job(int x)
{
  p[k++] = x ;
}


void job2(struct S s)
{
  q[k++] = s ;
}


void job3(struct S s)
{
  q[k].f[0] = s.f[0] ;
  q[k].f[1] = s.f[1] ;
  k++;
}


void caller(int x1 , int x2)
{
  job(x1);
  job(x2);
}


void caller2(struct S s1 , struct S s2)
{
  job2(s1);
  job2(s2);
}


void caller3(struct S s1 , struct S s2)
{
  job3(s1);
  job3(s2);
}




