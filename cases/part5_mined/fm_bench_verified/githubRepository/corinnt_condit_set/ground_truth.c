/*@ 
    requires valid_ptrs: \valid(a) && \valid_read(b); 
    requires diff_ptrs: \separated(a, b); 
    assigns val_to_first: *a; 
    ensures unchanged_second: \old(*b) == *b; 
    behavior second_true: 
        assumes *b; 
        ensures *a == 0; 
    behavior second_false: 
        assumes !*b; 
        ensures *a == \old(*a); 
    complete behaviors; 
    disjoint behaviors; 
*/
void reset_1st_if_2nd_is_true(int* a, int const* b){
    if(*b) *a = 0 ;
}
int main(){
    int a = 5 ;
    int x = 0 ;
    reset_1st_if_2nd_is_true(&a, &x);
    //@ assert a == 5 ;
    //@ assert x == 0 ;
    int const b = 1 ;
    reset_1st_if_2nd_is_true(&a, &b);
    //@ assert a == 0 ;
    //@ assert b == 1 ;
}