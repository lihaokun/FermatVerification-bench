/* run.config
   OPT: -wp-model +ref
*/

/* run.config_qualif
  OPT: -wp-model +ref
*/


void reset_5 (int *fp);

int reg_load[5];
int reg_add[5];


void load_5 (int * hp);


void add_5(int *gp);


void reset_1_5 (int (*rp) [5]) { reset_5(rp[0]);}


void load_1_5 (int (*lp) [5]) {load_5(lp[0]);}


void add_1_5 (int (*ap) [5]) {add_5(ap[0]);}

int t[20]; 
int tt[20][5];


void calls_on_array_dim_1 (void)
{
  load_5(t);
  reset_5(t);
  add_5(t);
}




void calls_on_array_dim_2_to_1 (void)
{
  load_5(tt[0]);
  reset_5(tt[0]);
  add_5(tt[0]);
}




void calls_on_array_dim_2 (void)
{
  load_1_5(tt);
  reset_1_5(tt);
  add_1_5(tt);
}




