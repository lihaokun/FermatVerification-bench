/* run.config
   OPT: -wp-model +ref -wp-msg-key refusage 
*/

/* run.config_qualif
   OPT: -wp-model +ref
*/


int ref_ctr(int * p); 


int r1 ; 


int call_ref_ctr (void)
{
  r1 = 4 ;
  return ref_ctr(&r1); 
}

int n1;


int call_ref_ctr2 (void)
{
  int * s ; s = &n1 ; 
  n1 = 4 ;
  if (ref_ctr(&n1) == 4) return *s; 
  else return n1;  
}

/*----------------------------------------*/


int ref_bd (int * q)
{
  return *q;
}

int r2 ; 


int call_ref_bd (void)
{
  r2 = 4 ;
  return ref_bd(&r2); 
}


int n2;


int call_ref_bd2 (void)
{
  int * p ; p = &n2 ; 
  n2 = 4 ;
  if (ref_bd(&n2) == 4) return *p; 
  else return n1;  
}



/*---------------------------------------*/



int ref_valid (int *p1);

int r7, n4 ;


int call_ref_valid(void)
{
  int * p ;
  r7 = 4; n4 = 4; 
  p = &n4; 
  return (ref_valid(&r7)+
          ref_valid(&n4));  

}

/*---------------------------------------*/


int no_ref_bd(int *q1)
{
  int * q ; q = q1+1 ; 
  return *q1;
}

int n5, nr6 ;


int call_no_ref_bd(void)
{
  int * p ;
  n5 = 4; nr6 = 4; 
  p = &n5; 
  return (no_ref_bd(&n5)+
          no_ref_bd(&nr6));  

}

/*---------------------------------------*/


int ref_ctr_nr(int *ref, int *ref1,int *ref2);

int r5,r6;
int f3,r4;


int call_ref_ctr_nr(void)
{ 
  f3 =2 ; r4 = 2  ; r6 = 2; 
  int k; 
  if (r5 == 0) k = f3 ; else k = r6; 
  return ref_ctr_nr(&r5,&k,&r4);
 
}

 
/*---------------------------------------*/


int ref_ctr_nstars(int **pp);



int call_ref_ctr_nstars(void)
{
  int x; 
  x = 4 ; 
  int * px = &x; 
  return ref_ctr_nstars(&px);
}


/*-------------------------------*/
int r0; 


int call_two_ref(void)
{
  r0 = 4; 
  return (ref_bd(&r0)+ref_ctr(&r0)) ; 
}

/*-------------------------------*/




int g (int *pg) {
  return ref_bd(pg);
}

/*-------------------------------*/


struct S { int tab[10]; int k;};
 
struct S array_in_struct_param(struct S sf){return sf;}

