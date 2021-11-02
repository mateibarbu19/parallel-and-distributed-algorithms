#include <math.h>
#include <pthread.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define TRUE 1  // daca numarul cautat este in interiorul intervalului
#define FALSE 0 // daca numarul cautat este in afara intervalului

typedef struct {
  size_t start, end;
} interval_t;

typedef struct {
  int thread_id;
  size_t N;
  unsigned int P;
  int number;
  int *v;
  char *inside;
  char *found;
  interval_t *segments;
  pthread_barrier_t *barrier;
} thread_data_t;



void *alloc_vector(size_t nmemb, size_t size) {
  int *v = malloc(size * nmemb);
  if (v == NULL) {
    fprintf(stderr, "alloc_vector: vector malloc failed!");
  }
  return v;
}

void *parallel_search(void *arg) {
  thread_data_t *data = (thread_data_t *)arg;
  size_t *start = &(data->segments[data->thread_id].start);
  size_t *end = &(data->segments[data->thread_id].end);
  unsigned int p;

  while (!(*data->found) && *start < *end) {
    p = 0;
    while (p < data->P && data->inside[p] == FALSE) {
      p++;
    }
    if (p < data->P) {
      *start = data->segments[p].start + data->thread_id * (data->segments[p].end - data->segments[p].start) / data->P;
      *end = data->segments[p].start + (data->thread_id + 1) * (data->segments[p].end - data->segments[p].start) / data->P;
    }
    if (p == data->P) {
      *end = *start;
    }

    pthread_barrier_wait(data->barrier);
    if (data->thread_id == 0) {
      for (p = 0; p < data->P; p++) {
        data->inside[p] = FALSE;
      }
    }

    pthread_barrier_wait(data->barrier);
    if (data->v[*start] == data->number || data->number == data->v[*end - 1]) {
      *(data->found) = 1;
    }

    if (data->v[*start] <= data->number || data->number <= data->v[*end - 1]) {
      data->inside[data->thread_id] = TRUE;
    }

    pthread_barrier_wait(data->barrier);
  }

  return NULL;
}

void display_vector(int *v, size_t N) {
  unsigned int display_width = 2 + log10(N);

  for (size_t i = 0; i < N; i++) {
    printf("%*i", display_width, v[i]);
  }

  printf("\n");
}


int main(int argc, char *argv[]) {
  unsigned int N, P, i;
  int *v, number;
  char *inside, found;
  pthread_t *threads;
  thread_data_t *args;
  interval_t *segments;
  pthread_barrier_t *barrier;

  if (argc < 4) {
    printf("Usage: ./parallel_binary_search N P number\n");
    return 1;
  }

  N = (unsigned int) atoi(argv[1]);
  P = (unsigned int) atoi(argv[2]);
  if (P < 2) {
    printf("Use a P greater than 1!\n");
    return 1;
  }
  number = atoi(argv[3]);

  v = (int *)alloc_vector(sizeof(*v), N);
  inside = alloc_vector(sizeof(*inside), P);
  threads = (pthread_t *)alloc_vector(sizeof(*threads), P);
  args = (thread_data_t *)alloc_vector(sizeof(*args), P);
  segments = (interval_t *)alloc_vector(sizeof(*segments), P);
  barrier = malloc(sizeof(*barrier));

  if (v == NULL || inside == NULL || threads == NULL || args == NULL || segments == NULL || barrier == NULL) {
    free(v);
    free(threads);
    free(inside);
    free(args);
    free(segments);
    free(barrier);
    fprintf(stderr, "Error on initializing values!");
    return 2;
  }

  for (i = 0; i < N; i++) {
    v[i] = i * 2;
  }

  display_vector(v, N);
  pthread_barrier_init(barrier, NULL, P);

  found = 0;
  for (i = 0; i < P; i++) {
    inside[i] = (v[0] <= number && number <= v[N - 1]);
    args[i].thread_id = i;
    args[i].N = N;
    args[i].P = P;
    args[i].number = number;
    args[i].found = &found;
    args[i].segments = segments;
    segments[i].start = 0;
    segments[i].end = N;
    args[i].v = v;
    args[i].inside = inside;
    args[i].barrier = barrier;

    int r = pthread_create(&threads[i], NULL, parallel_search, &args[i]);

    if (r) {
      fprintf(stderr, "An error occurred while creating thread %u.", i);
      exit(-1);
    }
  }

  for (i = 0; i < P; i++) {
    int r = pthread_join(threads[i], NULL);

    if (r) {
      fprintf(stderr, "An error occurred while creating thread %u.", i);
      exit(-2);
    }
  }

  if (found) {
    printf("Number %d was found in the array above.\n", number);
  } else {
    printf("Number %d was not found in the array above.\n", number);
  }

  free(v);
  free(threads);
  free(inside);
  free(args);
  free(segments);
  free(barrier);

  return 0;
}
