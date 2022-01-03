#include <math.h>
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define N 1000
#define MASTER 0

#define swap(a, b)        \
  ({                      \
	__typeof__(a) _aux = a; \
	a = b;                  \
	b = _aux;               \
  })

void *alloc_vector(size_t nmemb, size_t size) {
  void *v = calloc(nmemb, size);
  if (v == NULL) {
    fprintf(stderr, "alloc_vector: vector calloc failed!");
  }
  return v;
}

void compareVectors(int *a, int *b) {
  // DO NOT MODIFY
  int i;
  for (i = 0; i < N; i++) {
    if (a[i] != b[i]) {
      printf("Sorted incorrectly\n");
      return;
    }
  }
  printf("Sorted correctly\n");
}

void displayVector(int *v) {
  // DO NOT MODIFY
  for (int i = 0; i < N; i++) {
    printf("%d ", v[i]);
  }
  printf("\n");
}

int cmp(const void *a, const void *b) {
  // DO NOT MODIFY
  int A = *(int *)a;
  int B = *(int *)b;
  return A - B;
}

// https://stackoverflow.com/questions/664014/what-integer-hash-function-are-good-that-accepts-an-integer-hash-key
unsigned int hash(unsigned int x) {
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = (x >> 16) ^ x;
    return x & 0x7fffffff;
}


int main(int argc, char *argv[]) {
  int rank, i, j;
  int nProcesses;
  MPI_Init(&argc, &argv);
  int *posChunk, *pos, *v, *vQSort, *tmp;

  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &nProcesses);
  printf("Hello from %i/%i.\n", rank, nProcesses);

  v = alloc_vector(N, sizeof(*v));

  if (rank == MASTER) {
    vQSort = alloc_vector(N, sizeof(*v));
    pos = alloc_vector(N, sizeof(*pos));
    tmp = alloc_vector(N, sizeof(*tmp));

    // generate random vector
    srandom(42);
    for (int i = 0; i < N; i++) {
      // v[i] = hash(random() % 200);
      v[i] = hash(i);
    }
    displayVector(v);

    // make copy to check it against qsort
    // DO NOT MODIFY
    for (i = 0; i < N; i++) {
      vQSort[i] = v[i];
    }
    // sort the vector v
    qsort(vQSort, N, sizeof(int), cmp);
  }

  // send the vector to all processes
  MPI_Bcast(v, N, MPI_INT, MASTER, MPI_COMM_WORLD);
  // compute the positions
  int start = rank * N / nProcesses;
  int end = (rank + 1) * N / nProcesses;
  posChunk = alloc_vector(end - start, sizeof(*posChunk));
  for (int i = start; i < end; i++) {
    int count = 0;
    for (int j = 0; j < N; j++) {
      if (v[i] > v[j]) {
        count++;
      }
    }
    posChunk[i - start] = count;
  }

  if (rank == MASTER) {
    // local buffer
    for (i = start; i < end; i++) {
      pos[i] = posChunk[i];
    }
    for (int i = 1; i < nProcesses; i++) {
      MPI_Status status;
      int new_start = i * N / nProcesses;
      int chunk = (i + 1) * N / nProcesses - new_start;
      MPI_Recv(pos + new_start, chunk, MPI_INT, i, 1, MPI_COMM_WORLD, &status);
      printf("Master process [%d] received a posChunk from [%d].\n", MASTER, status.MPI_SOURCE);
    }
  } else {
    MPI_Send(posChunk, end - start, MPI_INT, MASTER, 1, MPI_COMM_WORLD);
    printf("Process with rank [%d], sent its posChunk.\n", rank);
  }

  if (rank == MASTER) {
    displayVector(pos);
    for (i = 0; i < N; i++) {
      tmp[pos[i]] = v[i];
    }
    for (i = 0; i < N; i++) {
      v[i] = tmp[i];
    }
    displayVector(v);
    compareVectors(v, vQSort);
    free(v);
    free(vQSort);
    free(pos);
    free(tmp);
  }

  free(posChunk);
  MPI_Finalize();
  return 0;
}
