#include "genetic_algorithm.h"
#include "pthread.h"
#include "stdio.h"
#include "utils.h"
#include <pthread.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  // array with all the objects that can be placed in the sack
  sack_object *objects = NULL;

  // number of objects
  int object_count = 0;

  // maximum weight that can be carried in the sack
  int sack_capacity = 0;

  // number of generations
  int generations_count = 0;

  unsigned int thread_count = read_input(
      &objects, &object_count, &sack_capacity, &generations_count, argc, argv);

  if (!thread_count) {
    return 0;
  }
  // thread_count = 1;

  pthread_t *threads =
      (pthread_t *)alloc_vector(thread_count, sizeof(*threads));
  thread_data *args =
      (thread_data *)alloc_vector(thread_count, sizeof(*args));
  pthread_barrier_t *barrier =
      (pthread_barrier_t *)alloc_vector(1, sizeof(*barrier));
  individual *current_generation =
      (individual *)alloc_vector(object_count, sizeof(individual));
  individual *next_generation =
      (individual *)alloc_vector(object_count, sizeof(individual));
  individual *tmp_generation =
      (individual *)alloc_vector(object_count, sizeof(individual));

  pthread_barrier_init(barrier, NULL, thread_count);

  for (unsigned int i = 0; i < thread_count; i++) {
    args[i].thread_id = i;
    args[i].thread_count = thread_count;
    args[i].objects = objects;
    args[i].object_count = object_count;
    args[i].generations_count = generations_count;
    args[i].sack_capacity = sack_capacity;
    args[i].barrier = barrier;
    args[i].current_generation = current_generation;
    args[i].next_generation = next_generation;
    args[i].tmp_generation = tmp_generation;

    int r = pthread_create(&threads[i], NULL, run_genetic_algorithm, &args[i]);

    if (r) {
      fprintf(stderr, "An error occurred while creating thread %u.\n", i);
      exit(-1);
    }
  }

  for (unsigned int i = 0; i < thread_count; i++) {
    int r = pthread_join(threads[i], NULL);

    if (r) {
      fprintf(stderr, "An error occurred while waiting for thread %u.\n", i);
      exit(-2);
    }
  }

  pthread_barrier_destroy(barrier);

  free(objects);
  free(threads);
  free(args);
  free(barrier);
  free(current_generation);
  free(next_generation);
  free(tmp_generation);

  return 0;
}
