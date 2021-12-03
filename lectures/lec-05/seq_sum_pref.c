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
    
    for (j = 1; j < n; j <<= 1) {
        print_array("%d ", v, sizeof(*v), n);
        for (i = n - 1; i >= j; i--) {
            v[i] += v[i - j];
        }
    }
    print_array("%d ", v, sizeof(*v), n);

    return 0;
}
