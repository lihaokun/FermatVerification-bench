struct S { int p,q; };


struct S negate(struct S *x)
{
  struct S r = *x;
  r.p = - r.p ;
  return r;
}
