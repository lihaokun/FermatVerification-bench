/*@ 
    requires valid_pointers: \valid(q) && \valid(r);
    requires \separated(q, r); 
    requires no_divide_by_zero: y != 0; 
    assigns *q, *r; 
    ensures *q == x / y; 
    ensures *r == x % y;  
*/
void div_rem(unsigned x, unsigned y, unsigned* q, unsigned* r){
     *q = x / y ;
     *r = x % y ;
}
int main() {
    unsigned x = 10, y = 3;
    unsigned q, r;
    div_rem(x, y, &q, &r);
    //@ assert q == 3 && r == 1;
    return 0;
}