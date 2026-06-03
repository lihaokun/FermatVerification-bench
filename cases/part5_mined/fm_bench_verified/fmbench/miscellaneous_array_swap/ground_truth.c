/*@
    requires n >= 0;
    requires 0 <= n1 < n && 0 <= n2 < n;
    requires \valid(arr+(0..n-1));
    ensures (arr[n2] == \old(arr[n1])) && (arr[n1] == \old(arr[n2]));
*/
void array_swap(int* arr, int n, int n1, int n2) {
    int temp = arr[n1];
    arr[n1] = arr[n2];
    arr[n2] = temp;
}
int main(){
    int arr[] = {1, 2, 3, 4, 5};
    array_swap(arr, 5, 1, 3);
    //@ assert arr[1] == 4;
    //@ assert arr[3] == 2;
}