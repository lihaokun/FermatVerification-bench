/* run.config
   OPT: -wp-rte -wp-msg-key state
*/
/* run.config_qualif
   DONTRUN:
*/

struct V {
	int* a ;
	unsigned* b ;
};

struct V* y ;


int* get_int(struct V* v);


unsigned* get_uint(struct V* v);

int main(void){
	struct V x ;
	x.a = get_int(&x);
	x.b = get_uint(&x);

	
}
