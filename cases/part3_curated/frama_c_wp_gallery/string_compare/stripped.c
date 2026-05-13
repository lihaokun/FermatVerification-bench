/* run.config
   OPT: -wp-no-print -wp-rte
*/

/* run.config_qualif
   OPT: -then -wp-rte -wp
*/

#include <stdint.h>
#include <string.h>
#include <stdio.h>


int stringCompare(const char* s1, const char* s2) {
    if (s1 == s2)
        return 0;

    
    while (*s1 == *(s2++))
    {
      if (*(s1++) == '\0') {
        
         return 0;
      }
    }

    
    return *(s1) - *(--s2);
}


size_t stringLength(const char* str) {
  const char* s = str ;

  
  while (*s++ != '\0');
  return --s - str;
}


int main(void) {

   const char hello[] = { 'h', 'e', 'l', 'l', 'o', '\0'};
   const char helli[] =  { 'h', 'e', 'l', 'l', 'i', '\0'};

   
   return stringCompare(hello, helli);
} 
