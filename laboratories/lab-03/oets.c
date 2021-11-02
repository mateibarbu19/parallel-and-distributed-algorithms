#include "utils.h"
#include <math.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

struct arg_t {
  unsigned int start, end;
  size_t N;
  int *array;
  pthread_barrier_t *barrier;
};

int *alloc_vector(size_t n) {
  int *v = malloc(n * sizeof(int));
  if (v == NULL) {
    fprintf(stderr, "alloc_vector: vector malloc failed!");
  }
  return v;
}

void display_vector(int *v, size_t N) {
  size_t i;
  unsigned int display_width = 2 + log10(N);

  for (i = 0; i < N; i++) {
    printf("%*i", display_width, v[i]);
  }

  printf("\n");
}

#ifndef SCALABILITY
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

int cmp(const void *a, const void *b) {
  int A = *(int *)a;
  int B = *(int *)b;
  return A - B;
}

void print(int *v, int *v_sorted, size_t N) {
  // print the current array
  // print the correct array
  // and compare them
  printf("v:\n");
  display_vector(v, N);
  printf("v_qsort:\n");
  display_vector(v_sorted, N);
  compare_vectors(v, v_sorted, N);
}
#endif

void *oets_p(void *arg) {
  struct arg_t *data = (struct arg_t *)arg;
  size_t k, i;


  for (k = 0; k < data->N; k++) {
    for (i = data->start; i < data->end; i += 2) {
      if (data->array[i] > data->array[i + 1]) {
        swap(data->array[i], data->array[i + 1]);
      }
    }
    pthread_barrier_wait(data->barrier);

    for (i = data->start + 1; i < data->end; i += 2) {
      if (data->array[i] > data->array[i + 1]) {
        swap(data->array[i], data->array[i + 1]);
      }
    }
    pthread_barrier_wait(data->barrier);
  }

  pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
  if (argc < 3) {
    printf("Usage: ./oets N P\n");
    return 2;
  }

  unsigned int N = atoi(argv[1]);
  unsigned int P = atoi(argv[2]);
  unsigned int i;
  pthread_t tid[P];
  pthread_barrier_t barrier;
  struct arg_t args[P];

  int *v = alloc_vector(N);
#ifndef SCALABILITY
  int *v_qsort = alloc_vector(N);
  if (v == NULL || v_qsort == NULL) {
    fprintf(stderr, "Error on initializing values!");
    free(v);
    free(v_qsort);
    return 2;
  }
#else
  if (v == NULL) {
    fprintf(stderr, "Error on initializing values!");
    return 2;
  }
#endif

  srand(42);
  for (i = 0; i < N; i++) {
    v[i] = rand() % N;
  }
#ifndef SCALABILITY
  for (i = 0; i < N; i++) {
    v_qsort[i] = v[i];
  }
  qsort(v_qsort, N, sizeof(int), cmp);
#endif

  pthread_barrier_init(&barrier, NULL, P);

  for (i = 0; i < P; i++) {
    args[i].start = make_even(i * N / P);
    args[i].end = min(make_even((i + 1) * N / P), N - 1);
    args[i].N = N;
    args[i].array = v;
    args[i].barrier = &barrier;
    int r = pthread_create(&tid[i], NULL, oets_p, &args[i]);

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
  pthread_barrier_destroy(&barrier);

#ifndef SCALABILITY
  print(v, v_qsort, N);
  free(v_qsort);
#else
  printf("v:\n");
  display_vector(v, N);
#endif
  free(v);

  return 0;
}
