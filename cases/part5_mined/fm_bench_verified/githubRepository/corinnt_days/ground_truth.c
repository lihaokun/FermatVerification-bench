/*@ 
    requires input_range: 1 <= month <= 12; 
    assigns \nothing; 
    ensures result_options: \result \in {28, 30, 31};  
    behavior day_28:
        assumes month == 2;
        ensures \result == 28; 
    behavior day_30:
        assumes month \in { 4, 6, 9, 11 } ;
        ensures \result == 30; 
     behavior days_31:
        assumes month \in {1, 3, 5, 7, 8, 10, 12};
        ensures \result == 31; 
    complete behaviors; 
    disjoint behaviors; 
*/
int day_of(int month){
    int days[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 } ;
    return days[month-1] ;
}
int main(){
    int days2 = day_of(2);
    //@ assert days2 == 28;
    int days7 = day_of(7);
    //@ assert days7 == 31;
}