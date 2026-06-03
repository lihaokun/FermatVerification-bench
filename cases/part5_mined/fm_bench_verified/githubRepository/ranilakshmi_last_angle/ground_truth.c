/*@
    behavior error:
        assumes first < 0 || first > 180 || second < 0 || second > 180 || first + second > 180;
        ensures \result == -1;
    behavior valid:
        assumes first >= 0 && first <= 180 && second >= 0 && second <= 180 && first + second <= 180;
        ensures \result == 180 - first - second;
    complete behaviors;
    disjoint behaviors;
*/
int last_angle(int first,int second){
    if(first < 0 || first > 180 || second < 0 || second > 180 || first + second > 180){
        return -1;
    }
    return 180 - first - second;
}
int main(){
    int first = 60;
    int second = 90;
    int result = last_angle(first,second);
    //@ assert result == 30;

    first = 100;
    second = 100;
    result = last_angle(first,second);
    //@ assert result == -1;
    return 0;
}