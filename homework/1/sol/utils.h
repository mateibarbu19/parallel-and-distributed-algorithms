#ifndef UTILS_H_
#define UTILS_H_
#include "individual.h"
#include "pthread.h"
#include "sack_object.h"
#include <stddef.h>

#define swap_val(a, b)                                                         \
  ({                                                                           \
    __typeof__(a) _aux = a;                                                    \
    a = b;                                                                     \
    b = _aux;                                                                  \
  })

#define min(a, b)                                                              \
  ({                                                                           \
    __typeof__(a) _a = (a);                                                    \
    __typeof__(b) _b = (b);                                                    \
    _a < _b ? _a : _b;                                                         \
  })

#define make_even(a)                                                           \
  ({                                                                           \
    __typeof__(a) _a = (a);                                                    \
    (_a % 2) ? _a + 1 : _a;                                                    \
  })

typedef int (*cmp_fct)(const void *a, const void *b);
typedef void (*swap_fct)(const void *a, const void *b);

void *alloc_vector(size_t nmemb, size_t size);

// generic merge algorithm
void merge(void *source, size_t mem_size, size_t start, size_t mid, size_t end,
           void *destination, cmp_fct cmp, swap_fct swap);

// generic parallel merge sort algorithm
void p_mergesort(void *v, void *v_new, size_t mem_size, size_t N, cmp_fct cmp,
                 swap_fct swap, unsigned int thread_id,
                 unsigned int thread_count, pthread_barrier_t *barrier);

#endif // UTILS_H_
