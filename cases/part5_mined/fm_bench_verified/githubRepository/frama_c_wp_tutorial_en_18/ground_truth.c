#include <stddef.h>
#include <limits.h>
/*@
	requires \valid_read(array+(0..length-1));
	assigns \nothing;
	behavior in:
		assumes \exists size_t off;0<=off<length&&array[off]==element;
		ensures array<=\result<array+length&&*\result==element;
	behavior notin:	
		assumes \forall size_t off;0<=off<length==>array[off]!=element;
		ensures \result==NULL;
	disjoint behaviors;	
	complete behaviors;	
*/
int* search(int* array, size_t length, int element) {
	/*@
		loop invariant 0<=i<=length;
		loop invariant \forall size_t j;0<=j<i==>array[j]!=element;
		loop assigns i;	
		loop variant length-i;
	*/
	for(size_t i = 0; i < length; i++) 
		if(array[i]==element) return &array[i];
	return NULL;
}
int main(){
    int array[] = {1,2,3,4,5};
    int *p = search(array,5,6);
    //@ assert p == NULL;
}