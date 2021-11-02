#include <math.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"

struct arg_t {
  unsigned int thread_id;
  int *v, *v_new;
  size_t N;
  unsigned int P;
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

void merge(int *source, size_t start, size_t mid, size_t end, int *destination) {
  size_t iA = start, iB = mid, i = start;

  while (iA < mid && iB < end) {
    if (source[iA] <= source[iB]) {
      destination[i++] = source[iA++];
    } else {
      destination[i++] = source[iB++];
    }
  }

  while (iA < mid) {
    destination[i++] = source[iA++];
  }

  while (iB < end) {
    destination[i++] = source[iB++];
  }
}

void *mergesort_p(void *arg) {
  struct arg_t *data = (struct arg_t *)arg;
  unsigned int start = data->thread_id * data->N / data->P; 
  unsigned int end = (data->thread_id + 1) * data->N / data->P;

  unsigned int width, i;
  for (width = 1; width < (end - start); width <<= 1) {
    for (i = start; i < end - width; i = i + 2 * width) {
      unsigned int last = min(i + 2 * width, end);
      merge(data->v, i, i + width, last, data->v_new);
      memcpy(data->v + i, data->v_new + i, (last - i) * sizeof(int));
    }
  }

  unsigned int l = 1;
  while (l < data->P && (data->thread_id & l) == 0) {
    pthread_barrier_wait(data->barrier);
    u_int mid_new = min(
                      (data->thread_id + l) * data->N / data->P,
                      data->N
                    );
    u_int end_new = min(
                      (data->thread_id + 2 * l) * data->N / data->P,
                      data->N
                    );
    merge(data->v, start, mid_new, end_new, data->v_new);
    memcpy(data->v + start, data->v_new + start, (end_new - start) * sizeof(int));
    l <<= 1;
  }

  while (l < data->P) {
    pthread_barrier_wait(data->barrier);
    l <<= 1;
  }

  pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
  if (argc < 3) {
    printf("Usage: ./merge N P\n");
    return 1;
  }

  unsigned int N = atoi(argv[1]);
  unsigned int P = atoi(argv[2]);
  unsigned int i;
  
  int *v = alloc_vector(N);
  int *v_new = alloc_vector(N);
#ifndef SCALABILITY
  int *v_qsort = alloc_vector(N);
  if (v == NULL || v_qsort == NULL || v_new == NULL) {
    fprintf(stderr, "Error on initializing values!");
    free(v);
    free(v_qsort);
    free(v_new);
    return 2;
  }
#else
  if (v == NULL || v_new == NULL) {
    fprintf(stderr, "Error on initializing values!");
    free(v);
    free(v_new);
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

  pthread_t tid[P];
  struct arg_t args[P];
  pthread_barrier_t barrier;
  pthread_barrier_init(&barrier, NULL, P);

  for (i = 0; i < P; i++) {
    args[i].thread_id = i;
    args[i].v = v;
    args[i].v_new = v_new;
    args[i].N = N;
    args[i].P = P;
    args[i].barrier = &barrier;
    int r = pthread_create(&tid[i], NULL, mergesort_p, &args[i]);

    if (r) {
      fprintf(stderr, "An error occurred while creating thread %u.\n", i);
      exit(-1);
    }
  }

  for (i = 0; i < P; i++) {
    int r = pthread_join(tid[i], NULL);

    if (r) {
      fprintf(stderr, "An error occurred while creating thread %u.\n", i);
      exit(-2);
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
  free(v_new);

  return 0;
}
