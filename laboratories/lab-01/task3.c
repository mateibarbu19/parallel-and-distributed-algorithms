#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

void *f(void *arg) {
  long id = *(long *)arg;
  for (unsigned short i = 0; i < 100; i++) {
    printf("Hello World din thread-ul %ld!\n", id);
  }
  pthread_exit(NULL);
}

int main(void) {
  pthread_t threads[NUM_THREADS];
  int r;
  long id;
  void *status;
  long ids[NUM_THREADS];

  for (id = 0; id < NUM_THREADS; id++) {
    ids[id] = id;
    r = pthread_create(&threads[id], NULL, f, &ids[id]);

    if (r) {
      fprintf(stderr, "An error occurred while creating thread %ld.", id);
      exit(-1);
    }
  }

  for (id = 0; id < NUM_THREADS; id++) {
    r = pthread_join(threads[id], &status);

    if (r) {
      fprintf(stderr, "An error occurred while waiting for thread %ld", id);
      exit(-1);
    }
  }

  pthread_exit(NULL);
}
