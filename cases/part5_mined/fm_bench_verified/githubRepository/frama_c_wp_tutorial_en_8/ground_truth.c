/*@
    requires 1 <= m <= 12;
    ensures m \in {2} ==> \result == 28;
    ensures m \in {1, 3, 5, 7, 8, 10, 12} ==> \result == 31;
    ensures m \in {4, 6, 9, 11} ==> \result == 30;
*/
int day_of(int m){
    int days[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    return days[m-1];
}
int main(){
    int m2 = day_of(2);
    //@ assert m2 == 28;
    int m1 = day_of(1);
    //@ assert m1 == 31;
    int m4 = day_of(4);
    //@ assert m4 == 30;
    return 0;
}