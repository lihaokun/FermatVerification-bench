/*@
    requires inputs: range: 0 < first < 180 &&
                            0 < second < 180;
    requires positive_remainder: first + second < 180; 
    ensures sum_180: first + second + \result == 180; 
    assigns \nothing; 
*/
int last_angle(int first, int second){
    return 180 - first - second ;
}
int main(){
    int first = 45;
    int second = 45;
    int result = last_angle(first, second);
    //@ assert result == 90;
}