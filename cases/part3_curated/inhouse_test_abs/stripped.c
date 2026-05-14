
int abs_val(int x) {
    if (x < 0) {
        return -x;
    }
    return x;
}


int double_abs(int n) {
    int r = abs_val(n);
    return r + r;
}
