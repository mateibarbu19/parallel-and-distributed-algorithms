#include <errno.h>
#include <pthread.h>
#include <sched.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define min(a, b)                                                              \
  ({                                                                           \
    __typeof__(a) _a = (a);                                                    \
    __typeof__(b) _b = (b);                                                    \
    _a < _b ? _a : _b;                                                         \
  })

struct arg_t {
  unsigned short id;
  unsigned int *arr;
  size_t start, end;
  unsigned int scalar;
};

void print_array(unsigned int *arr, size_t array_size) {
  for (size_t i = 0; i < array_size - 1; i++) {
    printf("%d ", arr[i]);
  }
  printf("%d\n", arr[array_size - 1]);
}

void *add_scalar(void *arg) {
  // https://stackoverflow.com/questions/1407786/how-to-set-cpu-affinity-of-a-particular-pthread
  struct arg_t args = *(struct arg_t *)arg;
  cpu_set_t cpuset;
  CPU_ZERO(&cpuset);
  CPU_SET(args.id, &cpuset);
  sched_setaffinity(0, sizeof(cpuset), &cpuset);
  for (size_t i = args.start; i < args.end; i++) {
    args.arr[i] += args.scalar;
  }
  pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
  pthread_t threads[NUM_THREADS];
  struct arg_t args[NUM_THREADS];
  void *status;
  int r;
  if (argc < 3) {
    fprintf(stderr, "Please specify the array's dimensions and constant to ");
    fprintf(stderr, "be added.\n");
    exit(-1);
  }

  int arg_size = atoi(argv[1]);
  int constant = atoi(argv[2]);
  if (arg_size < 0 || constant < 0) {
    perror("Please provide positive numbers!");
  }
  size_t array_size = (size_t)arg_size;

  unsigned int *arr = malloc(array_size * sizeof(int));
  for (size_t i = 0; i < array_size; i++) {
    arr[i] = i;
  }
  print_array(arr, array_size);

  for (unsigned short id = 0; id < NUM_THREADS; id++) {
    args[id].id = id;
    args[id].arr = arr;
    args[id].start = id * (double)array_size / NUM_THREADS;
    args[id].end = min((id + 1) * (double)array_size / NUM_THREADS, array_size);
    args[id].scalar = (unsigned int)constant;

    r = pthread_create(&threads[id], NULL, add_scalar, &args[id]);

    if (r) {
      printf("Eroare la crearea thread-ului %u\n", id);
      exit(-1);
    }
  }

  for (unsigned short id = 0; id < NUM_THREADS; id++) {
    r = pthread_join(threads[id], &status);

    if (r) {
      printf("An error occured while waiting for thread %hu to finish.\n", id);
      exit(-1);
    }
  }

  print_array(arr, array_size);

  pthread_exit(NULL);
}
