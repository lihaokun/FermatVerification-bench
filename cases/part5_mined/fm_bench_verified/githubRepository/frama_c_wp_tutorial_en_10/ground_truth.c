/*@ requires \valid(a) && \valid(b) && \valid(c);
    requires \separated(a, b, c);
    assigns *a, *b, *c;
    ensures *a <= *b <= *c;
    ensures { *a, *b, *c } == \old({ *a, *b, *c });
    ensures (\old(*a == *b < *c) || *a == *c < *b || *b == *c < *a) ==> *a == *b;
    ensures (\old(*a == *b > *c) || *a == *c > *b || *b == *c > *a) ==> *b == *c;
*/
void order_3(int* a, int* b, int* c){
    if(*a > *b){
        int tmp = *b;
        *b = *a;
        *a = tmp;
    }
    if(*a > *c){
        int tmp = *c;
        *c = *a;
        *a = tmp;
    }
    if(*b > *c){
        int tmp = *b;
        *b = *c;
        *c = tmp;
    }
}
int main(){
    int x = 10, y = 20, z = 15;
    order_3(&x, &y, &z);
    //@ assert x == 10 && y == 15 && z == 20;
    return 0;
}