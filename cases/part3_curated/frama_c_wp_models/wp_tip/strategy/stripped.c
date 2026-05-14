/* run.config
   DONTRUN:
*/

/* run.config_qualif
   OPT: -wp-status -wp-prover tip -wp-script dry -wp-strategy Prover
   OPT: -wp-status -wp-prover tip -wp-script dry -wp-strategy Eager
   OPT: -wp-status -wp-prover tip -wp-script dry -wp-strategy EagerRange
   OPT: -wp-status -wp-prover tip -wp-script dry -wp-strategy Lazy
 */







int target(unsigned x, unsigned y, unsigned z, unsigned t)
{
  return x+y+z+t;
}
