#include <limits.h>
/*@ 
    requires INT_MIN < c < INT_MAX;
    ensures character: function_result: 
            'a' <= c <= 'z' || 'A' <= c <= 'Z' 
            <==> \result == 1; 
    ensures non_character: function_result: 
            INT_MIN < c < 'A' || 
            'Z' < c < 'a'   ||
            'z' < c < INT_MAX  
            <==> \result == 0; 
    assigns \nothing; 
*/
int alphabet_letter(char c){
    if( ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z') ) return 1 ;
    else return 0 ;
}
int main(){
    int r ;
    r = alphabet_letter('x') ;
    //@ assert r ;
    r = alphabet_letter('H') ;
    //@ assert r ;
    r = alphabet_letter(' ') ;
    //@ assert !r ;
 }