/*
 * mini_sort.c — 5 functions, 3 levels
 *
 * Call graph:
 *   Level 0: swap, is_sorted
 *   Level 1: find_min_idx
 *   Level 2: selection_sort, sort_and_check
 */

/* Swap a[i] and a[j] */
void swap(int *a, int i, int j)
{
    int tmp = a[i];
    a[i] = a[j];
    a[j] = tmp;
}

/* Check if a[0..n-1] is sorted */
int is_sorted(const int *a, int n)
{
    for (int i = 0; i < n - 1; i++) {
        if (a[i] > a[i + 1])
            return 0;
    }
    return 1;
}

/* Find index of minimum in a[from..n-1] */
int find_min_idx(const int *a, int from, int n)
{
    int idx = from;
    for (int i = from + 1; i < n; i++) {
        if (a[i] < a[idx])
            idx = i;
    }
    return idx;
}

/* Selection sort a[0..n-1] in-place */
void selection_sort(int *a, int n)
{
    for (int i = 0; i < n - 1; i++) {
        int m = find_min_idx(a, i, n);
        if (m != i)
            swap(a, i, m);
    }
}

/* Sort and return 1 if sorted, 0 otherwise */
int sort_and_check(int *a, int n)
{
    selection_sort(a, n);
    return is_sorted(a, n);
}
