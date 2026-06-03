int grade;
/*@ requires mark >= 0;
    ensures \result == 0 || \result == 1 || \result == 2 || \result == 3 || \result == 4;
    behavior one:
        assumes mark >= 1 && mark <= 100;
        ensures \result == 1;
        assigns grade;
    behavior two:
        assumes mark >= 101 && mark <= 200;
        ensures \result == 2;
        assigns grade;
    behavior three:
        assumes mark >= 201 && mark <= 300;
        ensures \result == 3;
        assigns grade;
    behavior four:
        assumes mark >= 301 && mark <= 400;
        ensures \result == 4;
        assigns grade;
    behavior else:
        assumes mark <= 0 || mark > 400;
        ensures \result == 0;
        assigns grade;  
    complete behaviors;
    disjoint behaviors;
*/
int calculateGrade(int mark) {
    if (mark >= 1 && mark <= 100) {
        grade = 1;
        return grade;
    } else if (mark >= 101 && mark <= 200) {
        grade = 2;
        return grade;
    } else if (mark >= 201 && mark <= 300) {
        grade = 3;
        return grade;
    } else if (mark >= 301 && mark <= 400) {
        grade = 4;
        return grade;
    }
    return 0;
}
int main(){
    int mark = 150;
    int result = calculateGrade(mark);
    //@ assert result == 2;
    return 0;

}