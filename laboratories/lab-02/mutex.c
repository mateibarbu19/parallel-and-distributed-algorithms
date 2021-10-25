#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

static int a = 0;

#ifdef PROTECTED
void *f(void *arg) {
  pthread_mutex_t *mutex = (pthread_mutex_t *)arg;
  pthread_mutex_lock(mutex);
#else
void *f(__attribute__((unused)) void *arg) {
#endif

  a += 2;

#ifdef PROTECTED
  pthread_mutex_unlock(mutex);
#endif

  pthread_exit(NULL);
}

int main(void) {
  unsigned short i;
  int r;
  void *status;
  pthread_t threads[NUM_THREADS];

#ifdef PROTECTED
  pthread_mutex_t mutex;
  pthread_mutex_init(&mutex, NULL);
#endif

  for (i = 0; i < NUM_THREADS; i++) {
#ifdef PROTECTED
    r = pthread_create(&threads[i], NULL, f, &mutex);
#else
    r = pthread_create(&threads[i], NULL, f, NULL);
#endif

    if (r) {
      fprintf(stderr, "An error occured while creating thread %hu.", i);
      exit(-1);
    }
  }

  for (i = 0; i < NUM_THREADS; i++) {
    r = pthread_join(threads[i], &status);

    if (r) {
      fprintf(stderr, "An error occured while waiting for thread %hu", i);
      fprintf(stderr, "to finish.\n");
      exit(-1);
    }
  }

#ifdef PROTECTED
  pthread_mutex_destroy(&mutex);
#endif

  printf("a = %d\n", a);

  pthread_exit(NULL);
  return 0;
}
