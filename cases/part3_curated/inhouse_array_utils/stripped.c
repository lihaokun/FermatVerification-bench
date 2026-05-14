/*
 * array_utils.c — 10-function array utility library
 * Multi-level call graph for verify-program testing.
 *
 * Call graph (4 levels):
 *   Level 0 (leaf): swap, min, max, array_eq
 *   Level 1: find_min_idx, find_max_idx, is_sorted
 *   Level 2: selection_sort, reverse
 *   Level 3: sort_and_check
 */

#include <stddef.h>

/* === Level 0: Leaf functions === */

/* Swap two elements in an array */
void swap(int *a, int i, int j)
{
    int tmp = a[i];
    a[i] = a[j];
    a[j] = tmp;
}

/* Return the minimum of two integers */
int min(int x, int y)
{
    return (x <= y) ? x : y;
}

/* Return the maximum of two integers */
int max(int x, int y)
{
    return (x >= y) ? x : y;
}

/* Check if two arrays of length n are element-wise equal */
int array_eq(const int *a, const int *b, int n)
{
    for (int i = 0; i < n; i++) {
        if (a[i] != b[i])
            return 0;
    }
    return 1;
}

/* === Level 1: Use Level 0 === */

/* Find index of minimum element in a[from..n-1] */
int find_min_idx(const int *a, int from, int n)
{
    int idx = from;
    for (int i = from + 1; i < n; i++) {
        if (a[i] < a[idx])
            idx = i;
    }
    return idx;
}

/* Find index of maximum element in a[0..n-1] */
int find_max_idx(const int *a, int n)
{
    int idx = 0;
    for (int i = 1; i < n; i++) {
        if (a[i] > a[idx])
            idx = i;
    }
    return idx;
}

/* Check if array a[0..n-1] is sorted in non-decreasing order */
int is_sorted(const int *a, int n)
{
    for (int i = 0; i < n - 1; i++) {
        if (a[i] > a[i + 1])
            return 0;
    }
    return 1;
}

/* === Level 2: Use Level 0 + Level 1 === */

/* Selection sort: sort a[0..n-1] in-place */
void selection_sort(int *a, int n)
{
    for (int i = 0; i < n - 1; i++) {
        int m = find_min_idx(a, i, n);
        if (m != i)
            swap(a, i, m);
    }
}

/* Reverse array a[0..n-1] in-place */
void reverse(int *a, int n)
{
    for (int i = 0; i < n / 2; i++) {
        swap(a, i, n - 1 - i);
    }
}

/* === Level 3: Use Level 2 === */

/* Sort the array and return 1 if it is now sorted, 0 otherwise */
int sort_and_check(int *a, int n)
{
    selection_sort(a, n);
    return is_sorted(a, n);
}
