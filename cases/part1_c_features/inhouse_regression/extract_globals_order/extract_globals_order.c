/* Regression test: sandbox extraction must preserve global variable declaration order,
   collect transitive dependencies from initializer expressions, and only include
   types/globals that the target function actually depends on.

   Bug 1 (order): data_count references data in its initializer (sizeof(data)),
          but extraction outputs data_count before data → Parse Error.
   Bug 2 (missing dep): data_indirect is only referenced in data_count's initializer,
          not in bar's body. Collector doesn't traverse initializers → missing.
   Bug 3 (bloat): unrelated types/globals appear in the extracted file. */

typedef int element_t;

/* Unrelated type — should NOT appear in bar's extraction */
typedef struct { double x; double y; } point_t;

/* Unrelated global — should NOT appear in bar's extraction */
point_t origin = {0.0, 0.0};

/* data_indirect: ONLY referenced by data_count's initializer, NOT by bar */
static element_t data_indirect[8] = {10, 20, 30, 40, 50, 60, 70, 80};

/* data_count: references data_indirect via sizeof — must come AFTER data_indirect */
int const data_count = (int)(sizeof(data_indirect) / sizeof(data_indirect[0]));

/* bar: references data_count but NOT data_indirect */
int bar(int idx) {
    if (idx >= 0 && idx < data_count)
        return idx * 2;
    return -1;
}
