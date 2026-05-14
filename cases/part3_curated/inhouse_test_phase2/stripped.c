int counter = 0;
int max_val = 100;


int clamp(int x) {
    if (x > max_val) return max_val;
    return x;
}


void increment(int n) {
    counter += n;
}


int process(int x) {
    int val = clamp(x);
    increment(val);
    return val;
}

int main(void) {
    int a = process(50);
    int b = process(200);
    return a + b;
}
