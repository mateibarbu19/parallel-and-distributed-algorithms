#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

struct arg_t {
  unsigned int thread_id;
#ifdef PROTECTED
  pthread_barrier_t *barrier;
#endif
};

void *f(void *arg) {
  struct arg_t *input_arg = (struct arg_t *)arg;

  if (input_arg->thread_id == 1) {
    printf("1\n");
  }

#ifdef PROTECTED
  pthread_barrier_wait(input_arg->barrier);
#endif

  if (input_arg->thread_id == 0) {
    printf("2\n");
  }

  pthread_exit(NULL);
}

int main(void) {
  unsigned int i;
  int r;
  void *status;
  pthread_t threads[NUM_THREADS];
  struct arg_t arguments[NUM_THREADS];

#ifdef PROTECTED
  pthread_barrier_t barrier;
  pthread_barrier_init(&barrier, NULL, NUM_THREADS);
#endif

  for (i = 0; i < NUM_THREADS; i++) {
    arguments[i].thread_id = i;

#ifdef PROTECTED
    arguments[i].barrier = &barrier;
#endif

    r = pthread_create(&threads[i], NULL, f, &arguments[i]);

    if (r) {
      fprintf(stderr, "An error occured while creating thread %u.", i);
      exit(-1);
    }
  }

  for (i = 0; i < NUM_THREADS; i++) {
    r = pthread_join(threads[i], &status);

    if (r) {
      fprintf(stderr, "An error occured while waiting for thread %u", i);
      fprintf(stderr, "to finish.\n");
      exit(-1);
    }
  }

#ifdef PROTECTED
  pthread_barrier_destroy(&barrier);
#endif

  return 0;
}
