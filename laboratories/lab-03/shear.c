#include <math.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include "utils.h"

struct arg_t {
  size_t start, end, L;
  int **M;
  pthread_barrier_t *barrier;
};

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
    fprintf(stderr, "alloc_matrix: matrix calloc failed!");
  }
  for (i = 0; i < n; i++) {
    a[i] = calloc(m, sizeof(int));
    if (a[i] == NULL) {
      fprintf(stderr, "alloc_matrix: matrix line calloc failed!");
      free_matrix(a, i);
      return NULL;
    }
  }
  return a;
}

void display_matrix(int **M, size_t L) {
  size_t i, j;
  int display_width = 2 + 2 * log10(L);

  for (i = 0; i < L; i++) {
    for (j = 0; j < L; j++) {
      printf("%*i", display_width, M[i][j]);
    }
    printf("\n");
  }
}

void copy_matrix_in_vector(int *v, int **M, size_t L) {
  size_t i, j;
  for (i = 0; i < L; i++) {
    if (i % 2 == 0) {
      for (j = 0; j < L; j++) {
        v[i * L + j] = M[i][j];
      }
    } else {
      for (j = L; j > 0; j--) {
        v[i * L + (L - j)] = M[i][j - 1];
      }
    }
  }
}

int *alloc_vector(size_t n) {
  int *v = calloc(n, sizeof(int));
  if (v == NULL) {
    fprintf(stderr, "alloc_vector: vector calloc failed!");
  }
  return v;
}

void compare_vectors(int *a, int *b, size_t N) {
  size_t i = 0;
  while (i < N && a[i] == b[i]) {
    i++;
  }
  if (i == N) {
    printf("Correct sort\n");
  }
  else {
    printf("Incorrect sort\n");
  }
}

void display_vector(int *v, size_t N) {
  size_t i;
  unsigned int display_width = 2 + log10(N);

  for (i = 0; i < N; i++) {
    printf("%*i", display_width, v[i]);
  }

  printf("\n");
}

int cmp(const void *a, const void *b) {
  int A = *(int *)a;
  int B = *(int *)b;
  return A - B;
}

int cmpdesc(const void *a, const void *b) {
  int A = *(int *)a;
  int B = *(int *)b;
  return B - A;
}

void print(int ** M, int *v, int *v_sorted, size_t L) {
  printf("M:\n");
  // print the square matrix
  display_matrix(M, L);

  copy_matrix_in_vector(v, M, L);
  unsigned int N = L * L;
  // print the current array
  // print the correct array
  printf("v:\n");
  display_vector(v, N);
  printf("v_qsort:\n");
  display_vector(v_sorted, N);
  compare_vectors(v, v_sorted, N);
}

void *shear_p(void *arg) {
  struct arg_t *data = (struct arg_t *)arg;
  unsigned int i, j, k;
  unsigned int steps = ceil(2 * log2(data->L));

  int *aux = alloc_vector(data->L);
  if (aux == NULL) {
    fprintf(stderr, "shear: cannot allocate an auxiliary array");
    pthread_exit(NULL);
  }
  for (k = 0; k < steps; k++) {
    // sort the even indexed rows in ascending order
    // sort the odd indexed rows in descending order
    for (i = data->start; i < data->end; i++) {
      if (i & 1) {
        qsort(data->M[i], data->L, sizeof(int), cmpdesc); 
      } else {
        qsort(data->M[i], data->L, sizeof(int), cmp);
      }
    }

    pthread_barrier_wait(data->barrier);

    for (j = data->start; j < data->end; j++) {
      for (i = 0; i < data->L; i++) {
        aux[i] = data->M[i][j];
      }

      qsort(aux, data->L, sizeof(int), cmp);

      for (i = 0; i < data->L; i++) {
        data->M[i][j] = aux[i];
      }
    }

    pthread_barrier_wait(data->barrier);
  }
  free(aux);

  pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
  if (argc < 3) {
    printf("Usage: ./shear L P\n");
    printf("L = square matrix side length) P = number of threads\n");
    exit(1);
  }

  unsigned int L = atoi(argv[1]);
  unsigned int N = L * L;
  unsigned int P = atoi(argv[2]);
  unsigned int i, j;
  int *v = alloc_vector(N);
  int *v_qsort = alloc_vector(N);
  int **M = alloc_matrix(L, L);

  if (v == NULL || v_qsort == NULL || M == NULL) {
    fprintf(stderr, "Error on initializing values!");
    exit(2);
  }

  srand(42);
  for (i = 0; i < L; i++) {
    for (j = 0; j < L; j++) {
      M[i][j] = rand() % N;
    }
  }

  // use a standard library to sort
  copy_matrix_in_vector(v_qsort, M, L);
  qsort(v_qsort, N, sizeof(int), cmp);

  pthread_t tid[P];
  struct arg_t args[P];
  pthread_barrier_t barrier;
  pthread_barrier_init(&barrier, NULL, P);

  for (i = 0; i < P; i++) {
    args[i].start = i * L / P;
    args[i].end = (i + 1) * L / P;
    args[i].L = L;
    args[i].M = M;
    args[i].barrier = &barrier;
    int r = pthread_create(&tid[i], NULL, shear_p, &args[i]);

    if (r) {
      fprintf(stderr, "An error occurred while creating thread %u.", i);
      exit(-1);
    }
  }

  for (i = 0; i < P; i++) {
    int r = pthread_join(tid[i], NULL);

    if (r) {
      fprintf(stderr, "An error occurred while waiting for thread %u", i);
      fprintf(stderr, "to finish.\n");
      exit(-1);
    }
  }

  print(M, v, v_qsort, L);

  free(v);
  free(v_qsort);
  free_matrix(M, L);

  return 0;
}
