#include <cstdio>
#include <cstdlib>
#include <list>
#include <memory>
#include <pthread.h>
#include <queue>
#include <unistd.h>

#define MASTER 0

typedef struct thread_data_t {
  unsigned int thread_id;
  unsigned int thread_count;
  unsigned int sleep_time;
  std::shared_ptr<pthread_mutex_t> mutex;
  std::queue<unsigned int> &q;
} thread_data;

void *work(void *arg) {
  thread_data *targ = (thread_data *)arg;
  unsigned int thread_id = targ->thread_id;
  unsigned int thread_count = targ->thread_count;
  unsigned int sleep_time = targ->sleep_time;
    std::shared_ptr<pthread_mutex_t> mutex = targ->mutex;
  std::queue<unsigned int> &q = targ->q;

  if (thread_id != MASTER) {
    sleep(sleep_time);
    pthread_mutex_lock(mutex.get());
    q.push(thread_id);
    pthread_mutex_unlock(mutex.get());
  } else {
    unsigned int sum = 0;
    const unsigned int all_threads_sum = thread_count * (thread_count - 1) / 2;
    while (sum != all_threads_sum) {
      if (!q.empty()) {
        unsigned int id = q.front();

        pthread_mutex_lock(mutex.get());
        q.pop(); // CREW
        pthread_mutex_unlock(mutex.get());

        printf("Thread %u finished.\n", id);
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

  std::list<std::shared_ptr<pthread_t>> threads;
  std::list<thread_data> args;
  std::shared_ptr<pthread_mutex_t> mutex(new pthread_mutex_t);
  std::queue<unsigned int> q;

  pthread_mutex_init(mutex.get(), NULL);
  for (unsigned int i = 0; i < thread_count; i++) {
    pthread_t *thread = new pthread_t;
    threads.emplace_back(thread);
    args.push_back(
        {i, thread_count, (unsigned int)atoi(argv[1 + i]), mutex, q});

    int r = pthread_create(thread, NULL, work, &args.back());

    if (r) {
      fprintf(stderr, "An error occurred while creating thread %u.\n", i);
      exit(-1);
    }
  }

  for (std::shared_ptr<pthread_t> thread : threads) {
    int r = pthread_join(*thread, NULL);

    if (r) {
      fprintf(stderr, "An error occurred while waiting for thread %lu.\n",
              *thread);
      exit(-2);
    }
  }

  pthread_mutex_destroy(mutex.get());

  return 0;
}
