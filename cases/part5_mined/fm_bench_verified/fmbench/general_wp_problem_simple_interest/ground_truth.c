#include<limits.h>
/*@ 
    requires p>=5000;
    requires r> 0 && r <15;
    requires n > 0 && n < 5;
    ensures \result == p*n*r/100;
@*/
int simple(int p,int n,int r)
{
    int si;
    si = p*n*r/100;
    return si;
}
int main()
{
    int s = simple(10000, 3,10);
    //@ assert s == 3000;
    return 0;
}