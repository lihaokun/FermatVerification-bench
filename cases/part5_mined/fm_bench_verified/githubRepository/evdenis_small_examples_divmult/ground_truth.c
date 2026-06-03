/*@ 
    requires b != 0;
    requires b != -1;
    assigns \nothing;
    ensures \result == a/b * b + a % b;
 */
int divmult(int a, int b)
{
	return (a / b) * b + (a % b);
}
int main(){
    int a = 10;
    int b = 3;
    int c = divmult(a, b);
    //@ assert c == 10;
    return 0;
}