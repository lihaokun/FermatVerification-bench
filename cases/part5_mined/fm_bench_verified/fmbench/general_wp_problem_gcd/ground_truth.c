/*@
  logic integer gcd(integer a, integer b) = 
      (a==0)?b:
      (b==0)?a:
      (a==b)?a:
      (a>b)? gcd(a-b, b) : gcd(a, b-a);
*/
/*@
  requires a>0 && b>0;
  assigns \nothing;
  behavior exit1:
    assumes a == 0;
    ensures \result == b;
  behavior exit2:
    assumes b == 0;
    ensures \result == a;
  behavior exit3:
    assumes a == b;
    ensures \result == a;
  behavior exit4:
    assumes a > b;
    ensures \result == gcd(a-b,b);
  behavior exit5:
    assumes a < b;
    ensures \result == gcd(a,b-a);
  complete behaviors;
  disjoint behaviors;
*/
int gcd(int a, int b) {
    if (a == 0)
       return b;
    if (b == 0)
       return a;
    if (a == b)
        return a;
    if (a > b)
        return gcd(a-b, b);
    return gcd(a, b-a);
}
int main()
{
    int a = 98, b = 56;
    int c = gcd(a, b);
    //@ assert c == 14;
    return 0;
}