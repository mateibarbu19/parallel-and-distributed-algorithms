#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_THREADS 2

void *foo(__attribute__((unused)) void *arg) {
  printf("According to a Internet Engineering Task Force RFC, the word ");
  printf("FOO originated as a nonsense word with its earliest documented ");
  printf("use in the 1930s comic Smokey Stover by Bill Holman.\n");

  pthread_exit(NULL);
}

void *bar(__attribute__((unused)) void *arg) {
  printf("Legened has it that, one (undocumented) book describing the MIT");
  printf(" train room describes two buttons by the door labeled \"foo\" and");
  printf(" \"bar\".\n");

  pthread_exit(NULL);
}

int main(void) {
  pthread_t threads[NUM_THREADS];
  int r;
  long id;
  void *status;
  void *(*fun_array[2])(void *) = {foo, bar};

  for (id = 0; id < NUM_THREADS; id++) {
    r = pthread_create(&threads[id], NULL, fun_array[id], NULL);

    if (r) {
      fprintf(stderr, "An error occured while creating thread %ld.", id);
      exit(-1);
    }
  }

  for (id = 0; id < NUM_THREADS; id++) {
    r = pthread_join(threads[id], &status);

    if (r) {
      fprintf(stderr, "An error occured while waiting for thread %ld", id);
      fprintf(stderr, "to finish.\n");
      exit(-1);
    }
  }

  pthread_exit(NULL);
}
