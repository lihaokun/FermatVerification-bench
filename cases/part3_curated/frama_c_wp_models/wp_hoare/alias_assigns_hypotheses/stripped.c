/* run.config
   OPT:
   OPT:-wp-no-warn-memory-model -wp-check-memory-model -then -print
*/

/* run.config_qualif
	 DONT_RUN:
*/

int global[1];
int *g_alias;


void global_alias(void) {
	*g_alias = 1;
}


void global_no_alias(void) {
	*g_alias = 1;
}


void formal_alias(int* f_alias) {
	*f_alias = 1;
}


void formal_no_alias(int* f_alias) {
	*f_alias = 1;
}


void formal_alias_array(int (*alias_array)[2]){
  (*alias_array)[0] = 1;
  (*alias_array)[1] = 1;
}

// With field

struct X { int x; };


void field_alias(struct X* x){
  x->x = 1 ;
}

// With field, via set

// Through set:


void field_range_alias(struct X* x){
  x->x = 1 ;
}


void set_alias(int *f_alias) {
  *g_alias = 1;
}


// Through comprehension:


void comprehension_alias(void) {
  *g_alias = 1;
}

// Through union:


void union_alias(int *f_alias) {
  *g_alias = 1;
}
