int main() {
    int x = 1;
    int y = 0;
    /*@
    loop invariant 0 <= y <= 10;
    loop invariant 1 <= x <= 11;
    loop assigns y,x;
    */
    while (x <= 10) {
        y = 10 - x;
        x = x +1;
    }
    //@ assert y >= 0;
    //@ assert y <= 10;
}