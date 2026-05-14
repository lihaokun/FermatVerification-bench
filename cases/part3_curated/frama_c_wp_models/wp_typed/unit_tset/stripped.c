
void job(int *p,int n);

struct S {
  int size ;
  int value[50] ;
} s ;


void complex(void)
{
  job( & s.value[1] , s.size );
}
