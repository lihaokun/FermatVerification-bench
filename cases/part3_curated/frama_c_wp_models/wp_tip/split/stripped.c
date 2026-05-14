/* run.config
   DEPS: @PTEST_DEPS@ @WP_SESSION@/script/test_*.json
   OPT: -wp-par 1 -wp-no-print -wp-prover qed,script -wp-msg-key script @USING_WP_SESSION@
*/
/* run.config_qualif
   DONTRUN:
*/



int a, b;


void gen_P(void);


void gen_Q(void);


void gen_R(void);


void test_step_branch(void) {
  if (a < b) {
    gen_P();
  } else {
    gen_Q();
  }
}


void test_step_or(void) {}


void test_step_and(void) {}


void test_step_peq(void) {}


void test_step_pneq(void) {}


void test_step_neq(void) {}


void test_step_leq(void) {}


void test_step_lt(void) {}


void test_step_if(void) {}


void test_step_fa_if(void) {}


void test_step_fa_or(void) {}


void test_step_fa_and(void) {}


void test_inside_leq(void) {}


void test_inside_lt(void) {}


void test_inside_neq(void) {}


void test_goal_and(void) {}


void test_goal_eq(void) {}


void test_goal_neq(void) {}


void test_goal_if(void) {}


void test_goal_ex_and(void) {}


void test_goal_ex_or(void) {}


void test_goal_ex_if(void) {}


void test_goal_ex_imply(void) {}
