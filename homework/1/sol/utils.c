#include "utils.h"
#include "stdio.h"
#include "stdlib.h"
#include "pthread.h"

void *alloc_vector(size_t nmemb, size_t size) {
  int *v = calloc(nmemb, size);
  if (v == NULL) {
    fprintf(stderr, "alloc_vector: vector calloc failed!");
  }
  return v;
}


void merge(void *source, size_t mem_size, size_t start, size_t mid, size_t end,
           void *destination, cmp_fct cmp, swap_fct swap) {
  size_t iA = start;
  size_t iB = mid;
  size_t i = start;

  while (iA < mid && iB < end) {
    if (cmp(source + iA * mem_size, source + iB * mem_size) <= 0) {
      swap(source + iA * mem_size, destination + i * mem_size);
      iA++;
    } else {
      swap(source + iB * mem_size, destination + i * mem_size);
      iB++;
    }
    i++;
  }

  while (iA < mid) {
    swap(source + iA * mem_size, destination + i * mem_size);
    iA++;
    i++;
  }

  while (iB < end) {
    swap(source + iB * mem_size, destination + i * mem_size);
    iB++;
    i++;
  }
}

void p_mergesort(void *v, void *v_new, size_t mem_size, size_t N, cmp_fct cmp,
                 swap_fct swap, unsigned int thread_id,
                 unsigned int thread_count, pthread_barrier_t *barrier) {
  unsigned int width, i;
  unsigned int start = thread_id * N / thread_count;
  unsigned int end = (thread_id + 1) * N / thread_count;

  for (width = 1; width < (end - start); width <<= 1) {
    for (i = start; i < end - width; i = i + 2 * width) {
      unsigned int last = min(i + 2 * width, end);
      merge(v, mem_size, i, i + width, last, v_new, cmp, swap);
      for (unsigned int j = i; j < last; ++j) {
        swap(v_new + j * mem_size, v + j * mem_size);
      }
    }
  }

  unsigned int l = 1;
  while (l < thread_count && (thread_id & l) == 0) {
    pthread_barrier_wait(barrier);
    unsigned int mid_new = min((thread_id + l) * N / thread_count, N);
    unsigned int end_new = min((thread_id + 2 * l) * N / thread_count, N);
    merge(v, mem_size, start, mid_new, end_new, v_new, cmp, swap);
    for (unsigned int j = start; j < end_new; ++j) {
      swap(v_new + j * mem_size, v + j * mem_size);
    }
    l <<= 1;
  }

  while (l < thread_count) {
    pthread_barrier_wait(barrier);
    l <<= 1;
  }
  pthread_barrier_wait(barrier);
}