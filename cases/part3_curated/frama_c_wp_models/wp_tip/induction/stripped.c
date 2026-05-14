/* run.config
   DONTRUN:
*/

/* run.config_qualif
   DEPS: @PTEST_DEPS@ @WP_SESSION@/script/lemma_*.json
   OPT: -wp-prover script,Alt-Ergo -wp-timeout 1 @USING_WP_SESSION@
   DEPS: @PTEST_DEPS@ @WP_SESSION@/script/lemma_*.json
   OPT: -wp-prover script,Alt-Ergo -wp-timeout 1 @USING_WP_SESSION@
   DEPS: @PTEST_DEPS@ @WP_SESSION@/script/lemma_*.json
   OPT: -wp-prover script,Alt-Ergo -wp-timeout 1 @USING_WP_SESSION@
*/

// Script 0: induction on f(x) => success
// Script 1: induction on x => unsuccess
// Script 2: induction on y => unsuccess


