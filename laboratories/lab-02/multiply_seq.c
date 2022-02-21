#include <pthread.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

void multiply_seq(int **a, int **b, int **c, size_t N) {
  size_t i, j, k;
  for (i = 0; i < N; i++) {
    for (j = 0; j < N; j++) {
      int sum = 0;
      for (k = 0; k < N; k++) {
        sum += a[i][k] * b[k][j];
      }
      c[i][j] = sum; // appears to be a significat improvement when using -O0
    }
  }
}

void free_matrix(int **a, size_t n) {
  size_t i;
  for (i = 0; i < n; i++) {
    free(a[i]);
  }
  free(a);
}

int **alloc_matrix(size_t n, size_t m) {
  int **a;
  size_t i;
  a = calloc(n, sizeof(int *));
  if (a == NULL) {
    return NULL;
  }
  for (i = 0; i < n; i++) {
    a[i] = calloc(m, sizeof(int));
    if (a[i] == NULL) {
      free_matrix(a, i);
    }
  }
  return a;
}

void init_values(int **a, int **b, int **c, size_t N) {
  size_t i, j;
  for (i = 0; i < N; i++) {
    for (j = 0; j < N; j++) {
      c[i][j] = 0;

      if (i <= j) {
        a[i][j] = 1;
        b[i][j] = 1;
      } else {
        a[i][j] = 0;
        b[i][j] = 0;
      }
    }
  }
}

void print(int **mat, size_t N) {
  size_t i, j;

  for (i = 0; i < N; i++) {
    for (j = 0; j < N; j++) {
      printf("%i\t", mat[i][j]);
    }
    printf("\n");
  }
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    puts("Usage: ./multiply_outer N");
    return 1;
  }

  unsigned int N = atoi(argv[1]);

  int **a = alloc_matrix(N, N);
  if (a == NULL) {
    return 2;
  }
  int **b = alloc_matrix(N, N);
  if (b == NULL) {
    free_matrix(a, N);
    return 3;
  }
  int **c = alloc_matrix(N, N);
  if (c == NULL) {
    free_matrix(a, N);
    free_matrix(b, N);
    return 4;
  }

  init_values(a, b, c, N);

  multiply_seq(a, b, c, N);

  print(c, N);

  return 0;
}
