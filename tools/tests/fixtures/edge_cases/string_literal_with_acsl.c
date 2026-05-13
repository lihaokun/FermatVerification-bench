/* Edge case fixture: ACSL-looking syntax inside string / char literals must NOT
   be detected as ACSL blocks. */

#include <stdio.h>

/*@ requires x >= 0;
    ensures \result >= 0; */
int real_acsl_block(int x) {
    // Real ACSL above. The string below has fake /*@ and should be left alone.
    const char *fake1 = "this string contains /*@ requires fake; */ as text";
    const char *fake2 = "//@ assert fake; this is also string";
    char fake3 = '/';   // Single char, not start of comment
    //@ assert real_line_acsl: x == x;
    /*@ assert another_real_block: x >= 0; */
    return x;
}

/* Regular C comment, not ACSL */
// Regular C line comment, not ACSL

/*@ ensures \result == 42; */
int trailing_acsl(void) {
    return 42;
}
