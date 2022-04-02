#include <cstdlib>
#include <pthread.h>
#include <queue>
#include <stdio.h>
#include <unistd.h>
#include <vector>

#define MASTER 0

typedef struct thread_data_t {
  unsigned int thread_id;
  unsigned int thread_count;
  unsigned int sleep_time;
  pthread_mutex_t *mutex;
  std::queue<unsigned int> *q;
} thread_data;

void *alloc_vector(size_t nmemb, size_t size) {
  void *v = calloc(nmemb, size);
  if (v == NULL) {
    fprintf(stderr, "alloc_vector: vector calloc failed!");
  }
  return v;
}

void *work(void *arg) {
  thread_data *targ = (thread_data *)arg;
  unsigned int thread_id = targ->thread_id;
  unsigned int thread_count = targ->thread_count;
  unsigned int sleep_time = targ->sleep_time;
  pthread_mutex_t *mutex = targ->mutex;
  std::queue<unsigned int> *q = targ->q;

  if (thread_id != MASTER) {
    sleep(sleep_time);
    pthread_mutex_lock(mutex);
    q->push(thread_id);
    pthread_mutex_unlock(mutex);
  } else {
    unsigned int sum = 0;
    const unsigned int all_threads_sum = thread_count * (thread_count - 1) / 2;
    while (sum != all_threads_sum) {
      if (!q->empty()) {
        auto id = q->front();

        pthread_mutex_lock(mutex);
        q->pop(); // CREW
        pthread_mutex_unlock(mutex);

        printf("Thread-ul %u a terminat\n", id);
        sum += id;
      }
    }
  }

  return NULL;
}

int main(int argc, char *argv[]) {
  if (argc <= 2 || argc > 9) {
    printf("Usage: %s thread_count sleep_time_0 sleep_time_1 ... ", argv[0]);
    printf("sleep_time_(thread_count -1)\n");
    return 1;
  }

  unsigned int thread_count = (unsigned int)atoi(argv[1]);

  pthread_t *threads =
      (pthread_t *)alloc_vector(thread_count, sizeof(*threads));
  thread_data *args = (thread_data *)alloc_vector(thread_count, sizeof(*args));
  pthread_mutex_t *mutex = (pthread_mutex_t *)alloc_vector(1, sizeof(*mutex));
  std::queue<unsigned int> q;

  pthread_mutex_init(mutex, NULL);
  for (unsigned int i = 0; i < thread_count; i++) {
    args[i].thread_id = i;
    args[i].thread_count = thread_count;
    args[i].sleep_time = (unsigned int)atoi(argv[1 + i]);
    args[i].mutex = mutex;
    args[i].q = &q;

    int r = pthread_create(&threads[i], NULL, work, &args[i]);

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

  pthread_mutex_destroy(mutex);
  free(threads);
  free(args);
  free(mutex);

  return 0;
}