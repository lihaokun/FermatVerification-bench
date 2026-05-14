/* run.config
   OPT: -wp-model +ref
*/

/* run.config_qualif
  OPT: -wp-model +ref
*/

struct T {int a; int b;};


void reset (struct T *p) {p->a = 0;}

struct T st ;


int call_reset (void)
{ reset(&st) ; return (st.a);}



void reset_5 (struct T *fp);


struct T ts[10];


void call_reset_5 (void)
{ reset_5(ts) ; return ;}


struct T * tps[10];


void call_reset_5_tps(void)
{ reset_5(tps[9]); return;}




void reset_1_5 (struct T (*rp) [5]);

struct T smatrix [20] [5] ;


void call_reset_1_5(void)
{reset_1_5(smatrix); return;}



void call_reset_5_dim2(void)
{reset_5(smatrix[1]); return;}

/*-------------------------------------------------------*/

struct S { int tab[10]; int k;};
struct S s;

int reg_load[5];

void load_5 (int * hp);

 
void call_on_array_in_struct_global (void){load_5(s.tab);}

 
struct S call_array_in_struct_param(struct S sf){load_5(sf.tab); return sf;}

