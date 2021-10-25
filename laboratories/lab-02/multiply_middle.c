#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define min(a, b)                                                              \
  ({                                                                           \
    __typeof__(a) _a = (a);                                                    \
    __typeof__(b) _b = (b);                                                    \
    _a < _b ? _a : _b;                                                         \
  })

struct arg_t {
  int **a;
  int **b;
  int **c;
  size_t N;
  size_t column_start;
  size_t column_end;
};

void *multiply_middle(void *arg) {
  struct arg_t *m_arg = (struct arg_t *)arg;

  size_t i, j, k;
  for (i = 0; i < m_arg->N; i++) {
    for (j = m_arg->column_start; j < m_arg->column_end; j++) {
      for (k = 0; k < m_arg->N; k++) {
        m_arg->c[i][j] += m_arg->a[i][k] * m_arg->b[k][j];
      }
    }
  }

  pthread_exit(NULL);
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
  if (argc != 3) {
    printf("Usage: ./multiply_outer N P\n");
    return 1;
  }

  unsigned int N = atoi(argv[1]);
  unsigned int P = atoi(argv[2]);
  unsigned int i;

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

  pthread_t tid[P];
  struct arg_t args[P];

  for (i = 0; i < P; i++) {
    args[i].a = a;
    args[i].b = b;
    args[i].c = c;
    args[i].N = N;
    args[i].column_start = i * N / P;
    args[i].column_end = min((i + 1) * N / P, N);
    int r = pthread_create(&tid[i], NULL, multiply_middle, &args[i]);

    if (r) {
      fprintf(stderr, "An error occured while creating thread %u.", i);
      exit(-1);
    }
  }

  for (i = 0; i < P; i++) {
    int r = pthread_join(tid[i], NULL);

    if (r) {
      fprintf(stderr, "An error occured while waiting for thread %u", i);
      fprintf(stderr, "to finish.\n");
      exit(-1);
    }
  }

  print(c, N);

  return 0;
}
