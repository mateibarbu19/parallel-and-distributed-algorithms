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

void compare_vectors(int *a, int *b, size_t N) {
  size_t i = 0;
  while (i < N) {
    if (a[i] != b[i]) {
      printf("Incorrect sort\n");
      return;
    }
  }

  printf("Correct sort\n");
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

void print(int *v, int *vSorted, size_t N) {
  // print the correct array
  // print the current array
  // and compare them
  printf("v:\n");
  display_vector(v, N);
  printf("vQSort:\n");
  display_vector(vSorted, N);
  compare_vectors(v, vSorted, N);
}

void *oets(void *arg) {
  struct arg_t *data = (struct arg_t *)arg;
  size_t k, i;

  for (k = 0; k < data->N; k++) {
    for (i = data->start; i < data->end - 1; i += 2) {
      if (data->array[i] > data->array[i + 1]) {
        printf("i:%zu %d %d\n", i, data->array[i], data->array[i + 1]);
        swap(data->array[i], data->array[i + 1]);
      }
    }
    pthread_barrier_wait(data->barrier);

    for (i = data->start + 1; i < data->end - 1; i += 2) {
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
    exit(1);
  }

  unsigned int N = atoi(argv[1]);
  unsigned int P = atoi(argv[2]);
  unsigned int i;
  pthread_t tid[P];
  pthread_barrier_t barrier;
  struct arg_t args[P];

  int *v = malloc(sizeof(int) * N);
  if (v == NULL) {
    fprintf(stderr, "Error on malloc!");
    exit(1);
  }

  int *vQSort = malloc(sizeof(int) * N);
  if (vQSort == NULL) {
    fprintf(stderr, "Error on malloc!");
    exit(2);
  }

  srand(42);
  for (i = 0; i < N; i++) {
    vQSort[i] = v[i] = rand() % N;
  }
  qsort(vQSort, N, sizeof(int), cmp);

  pthread_barrier_init(&barrier, NULL, P);

  // se creeaza thread-urile
  for (i = 0; i < P; i++) {
    args[i].start = i * (N / P);
    args[i].start -= args[i].start & 1;
    args[i].end = min((i + 1) * N / P, N);
    args[i].end -= args[i].end & 1;
    if (i == P - 1) {
      args[i].end = N;
    }
    printf("%d %d\n", args[i].start, args[i].end);
    args[i].N = N;
    args[i].array = v;
    args[i].barrier = &barrier;
    int r = pthread_create(&tid[i], NULL, oets, &args[i]);

    if (r) {
      fprintf(stderr, "An error occured while creating thread %u.", i);
      exit(-1);
    }
  }

  // se asteapta thread-urile
  for (i = 0; i < P; i++) {
    int r = pthread_join(tid[i], NULL);

    if (r) {
      fprintf(stderr, "An error occured while waiting for thread %u", i);
      fprintf(stderr, "to finish.\n");
      exit(-1);
    }
  }

  print(v, vQSort, N);

  free(v);
  free(vQSort);

  return 0;
}
