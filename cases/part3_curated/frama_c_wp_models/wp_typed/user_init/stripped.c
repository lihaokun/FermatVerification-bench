/* run.config_qualif

   OPT: -wp-prop=-lack,-tactic -wp-timeout 10
   OPT: -wp-prop=tactic -wp-timeout 5 -wp-auto=wp:split,wp:range -wp-prover=tip,Alt-Ergo -wp-script dry
   OPT: -wp-prop=lack
 */

void init( int * a , int n , int v )
{
  
  for (int i = 0 ; i < n ; i++) a[i] = v ;
}
//-------------------------
int t1[10];

void init_t1(int v) {
  unsigned i;
  
  for (i = 0 ; i < 10 ; i++) t1[i] = v ;
}
//-------------------------
int t2[10][20];

void init_t2_v1(int v) {

  unsigned i,j;
  
  for(i = 0; i <= 9; i++) {
    
    for(j = 0; j <= 19; j++) {
      t2[i][j] = v;
    }
    
    ;
  }
  
  ;
}
//-------------------------

void init_t2_v2(int v) {

  unsigned i,j;
  
  for(i = 0; i <= 9; i++) {
    
    for(j = 0; j <= 19; j++) {
      t2[i][j] = v;
    }
    
    ;
  }
  
  ;
}
//-------------------------



void init_t2_v3(int v) {

  unsigned i,j;
  
  for(i = 0; i <= 9; i++) {
    
    for(j = 0; j <= 19; j++) {
      t2[i][j] = v;
    }
    
    ;
  }
  
  ;
}
//-------------------------

void init_t2_bis_v1(int v) {

  unsigned i;
  
  for(i = 0; i <= 9; i++) {
    init(&t2[i][0], 20, v);
    
  }
}
//-------------------------

void init_t2_bis_v2(int v) {

  unsigned i;
  
  for(i = 0; i <= 9; i++) {
    init(&t2[i][0], 20, v);
    
    ;
  }
}
