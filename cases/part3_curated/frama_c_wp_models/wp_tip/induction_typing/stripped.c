/* run.config
   DEPS: @PTEST_DEPS@ @WP_SESSION@/script/function_*.json
   OPT: -wp-par 1 -wp-prop X -wp-no-print -wp-prover qed,script -wp-msg-key script @USING_WP_SESSION@
*/
/* run.config_qualif
   DONTRUN:
*/

/* This test is meant to check that we do not generate a ill-formed VC with the
   induction tactic. Here, the bug happened when triggering an induction on i
   (i was replaced with true) when proving that X is preserved . The example is
   complex because we need to have some State variable for i. */

extern int LIST;
extern unsigned int cpt;
extern unsigned int A[];




void f(unsigned int i);



void function(unsigned int Max)
{
	unsigned int i = 0;
	
	while (i < Max) {
		f(i);
		i ++;
	}

  return;
}
