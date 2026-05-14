/* Regression test: sandbox must not clobber main instance state.
   After create_sandbox, list_functions on main must still return
   both foo and bar (not just the sandbox's extracted function). */

int bar(int x) { return x + 1; }

int foo(int x) { return bar(x) * 2; }
