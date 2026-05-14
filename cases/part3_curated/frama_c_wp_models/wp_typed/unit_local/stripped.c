/* run.config
   OPT:
   OPT: -wp-model +raw
 */

/* run.config_qualif
   OPT:
   OPT: -wp-model +raw
 */


void foo(void)
{
  int a;
  int* pa;
  pa = &a;
  *pa = 1;
}


void bar(void)
{
  int a;
  a = 1;
}
