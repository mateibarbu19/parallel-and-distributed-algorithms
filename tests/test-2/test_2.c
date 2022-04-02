#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>

// General utils functions
#define min(a, b)                                                              \
  ({                                                                           \
    __typeof__(a) _a = (a);                                                    \
    __typeof__(b) _b = (b);                                                    \
    _a < _b ? _a : _b;                                                         \
  })

#define max(a, b)                                                              \
  ({                                                                           \
    __typeof__(a) _a = (a);                                                    \
    __typeof__(b) _b = (b);                                                    \
    _a > _b ? _a : _b;                                                         \
  })

void *alloc_vector(size_t nmemb, size_t size) {
  void *v = calloc(nmemb, size);
  if (v == NULL) {
    fprintf(stderr, "alloc_vector: vector calloc failed!");
  }
  return v;
}

// Used for debugging
static const int DEBUG = 0;
// Using the preprocessor was another option

int main(int argc, char *argv[]) {
  int N, R;
  int buffer, value;
  MPI_Status status;

  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &N); // Total number of processes.
  MPI_Comm_rank(MPI_COMM_WORLD, &R); // The current process ID / Rank.

  // Checks the number of processes allowed.
  if (N < 2 || N > 12) {
    printf("Wrong number of processes!");
    MPI_Finalize();
    return 1;
  }

  value = 2 * (N - R);

  for (int step = 0; step < N; step++) {
    if (step % 2 == 0) {
      if (R % 2 == 0 && R + 1 < N) {
        MPI_Send(&value, 1, MPI_INT, R + 1, 1, MPI_COMM_WORLD);

        if (DEBUG) {
          printf("Process with rank [%d], sent %d to [%d].\n", R, value, R + 1);
        }

        MPI_Recv(&buffer, 1, MPI_INT, R + 1, 1, MPI_COMM_WORLD, &status);

        if (DEBUG) {
          printf("Process with rank [%d], received %d from [%d].\n", R, buffer,
                 status.MPI_SOURCE);
        }

        value = min(value, buffer);
      } else if (R % 2 == 1 && R - 1 >= 0) { // in fact only R % 2 is necessary
        MPI_Send(&value, 1, MPI_INT, R - 1, 1, MPI_COMM_WORLD);

        if (DEBUG) {
          printf("Process with rank [%d], sent %d to [%d].\n", R, value, R - 1);
        }

        MPI_Recv(&buffer, 1, MPI_INT, R - 1, 1, MPI_COMM_WORLD, &status);

        if (DEBUG) {
          printf("Process with rank [%d], received %d from [%d].\n", R, buffer,
                 status.MPI_SOURCE);
        }

        value = max(value, buffer);
      }
    } else /* if (step % 2 == 1) */ {
      if (R % 2 == 1 && R + 1 < N) {
        MPI_Send(&value, 1, MPI_INT, R + 1, 1, MPI_COMM_WORLD);

        if (DEBUG) {
          printf("Process with rank [%d], sent %d to [%d].\n", R, value, R + 1);
        }

        MPI_Recv(&buffer, 1, MPI_INT, R + 1, 1, MPI_COMM_WORLD, &status);

        if (DEBUG) {
          printf("Process with rank [%d], received %d from [%d].\n", R, buffer,
                 status.MPI_SOURCE);
        }

        value = min(value, buffer);
      } else if(R % 2 == 0 && R - 1 >= 0) {
        MPI_Send(&value, 1, MPI_INT, R - 1, 1, MPI_COMM_WORLD);

        if (DEBUG) {
          printf("Process with rank [%d], sent %d to [%d].\n", R, value, R - 1);
        }

        MPI_Recv(&buffer, 1, MPI_INT, R - 1, 1, MPI_COMM_WORLD, &status);

        if (DEBUG) {
          printf("Process with rank [%d], received %d from [%d].\n", R, buffer,
                 status.MPI_SOURCE);
        }

        value = max(value, buffer);
      }
    }
  }

  printf("%d - %d\n", R, value);

  int *v = (int *) alloc_vector(N - R, sizeof(*v));
  v[0] = value;

  if (R > 0) {
    MPI_Send(&value, 1, MPI_INT, R - 1, 1, MPI_COMM_WORLD);

    if (DEBUG) {
      printf("Process with rank [%d], sent %d to [%d].\n", R, value, R - 1);
    }
  }

  for (int i = R + 1; i < N; i++) {
    MPI_Recv(&buffer, 1, MPI_INT, R + 1, 1, MPI_COMM_WORLD, &status);

    if (DEBUG) {
      printf("Process with rank [%d], received %d from [%d].\n", R, buffer,
              status.MPI_SOURCE);
    }
    v[i - R] = buffer;

    if (R > 0) {
      MPI_Send(&buffer, 1, MPI_INT, R - 1, 1, MPI_COMM_WORLD);

      if (DEBUG) {
        printf("Process with rank [%d], sent %d to [%d].\n", R, value, R - 1);
      }
    }
  }

  if (R == 0) {
    for (int i = 0; i < N; i++) {
      printf("%d ", v[i]);
    }
    printf("\n");
  }

  free(v);

  MPI_Finalize();
  return 0;
}