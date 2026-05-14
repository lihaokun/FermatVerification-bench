/* run.config
   DEPS: @PTEST_DEPS@ @WP_SESSION@/script/clear_*.json
   OPT: -wp-par 1 -wp-no-print -wp-prover qed,script -wp-msg-key script @USING_WP_SESSION@
*/
/* run.config_qualif
   DONTRUN:
*/



int a = 42, b;


void clear(void) {
  if (a < b) {
    a++;
  } else {
    b--;
  }
}

void clear_in_step(void){
  
  
}
