/* run.config_qualif
   OPT: -wp-model region -wp-timeout 5
 */

union U {
    short s[2];
    int i;
};


void f (union U *p) {
    p->i = 42;
    p->s[0] = -1;
    p->s[1] = -1;
    
    (p->i)++;
}
