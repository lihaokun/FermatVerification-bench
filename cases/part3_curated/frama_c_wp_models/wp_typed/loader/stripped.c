/* run.config_qualif
   DONTRUN:
*/



struct S {
  int f;
  int g[4];
  int m[3][5];
};


struct S load(struct S *x, int k, int i, int j)
{
  return *x;
}
