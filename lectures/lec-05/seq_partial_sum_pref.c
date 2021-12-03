#include <stdio.h>

int v[100];

void print_array(const char *fmt, void *a, size_t mem_size, size_t n_mem) {
    char *v = (char *)a;
    for (size_t i = 0; i < n_mem; i ++) {
        printf(fmt, v[i * mem_size]);
    }
    printf("\n");
}

int main() {
    unsigned int n;
    size_t i, j;
    scanf("%d", &n);
    for (i = 0; i < n; i++) {
        scanf("%d", &v[i]);
    }
    
    // works only for powers of two
    for (j = 1; j < n; j <<= 1) {
        print_array("%d ", v, sizeof(*v), n);
        for (i = n; i >= 1; i--) {
            if (i % (2 * j) == 0) {
                v[i - 1] += v[i - 1 - j];
            }
        }
    }
    print_array("%d ", v, sizeof(*v), n);

    return 0;
}
