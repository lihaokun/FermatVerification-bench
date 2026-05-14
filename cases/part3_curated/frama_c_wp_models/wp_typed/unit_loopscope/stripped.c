/* run.config_qualif
   OPT:
   OPT: -wp-model +ref
*/




void f(unsigned int * written)
{
 int n = 1;
 *written = (unsigned int)0;
 
 while (n < 10) {
   n++;
 }
 return;
}
