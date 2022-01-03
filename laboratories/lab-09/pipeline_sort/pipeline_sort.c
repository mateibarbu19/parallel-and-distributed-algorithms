#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MASTER 0

void *alloc_vector(size_t nmemb, size_t size) {
  void *v = calloc(nmemb, size);
  if (v == NULL) {
    fprintf(stderr, "alloc_vector: vector calloc failed!");
  }
  return v;
}

void compareVectors(int *a, int *b, size_t N) {
  // DO NOT MODIFY
  for (size_t i = 0; i < N; i++) {
    if (a[i] != b[i]) {
      printf("Sorted incorrectly\n");
      return;
    }
  }
  printf("Sorted correctly\n");
}

void displayVector(int *v, size_t N) {
  // DO NOT MODIFY
  for (size_t i = 0; i < N; i++) {
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

// Use 'mpirun -np 20 --oversubscribe ./pipeline_sort' to run the application
// with more processes
int main(int argc, char *argv[]) {
  int rank;
  int nProcesses;
  MPI_Status status;
  MPI_Init(&argc, &argv);

  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &nProcesses);
  printf("Hello from %d/%d.\n", rank, nProcesses);
  
  int value = INT_MAX;
  int *v;
  if (rank == MASTER) { // This code is run by a single process
    v = (int *)alloc_vector((nProcesses - 1), sizeof(*v));
    int *vQSort = (int *)alloc_vector((nProcesses - 1), sizeof(*v));

    // generate the vector v with random values
    // DO NOT MODIFY
    srandom(42);
    for (int i = 0; i < nProcesses - 1; i++) {
      v[i] = random() % 200;
    }
    displayVector(v, nProcesses - 1);

    // make copy to check it against qsort
    // DO NOT MODIFY
    for (int i = 0; i < nProcesses - 1; i++) {
      vQSort[i] = v[i];
    }
    qsort(vQSort, nProcesses - 1, sizeof(int), cmp);

    // DONE: sort the vector v
    for (int i = 0; i < nProcesses - 1; i++) {
      MPI_Send(&v[i], 1, MPI_INT, rank + 1, 1, MPI_COMM_WORLD);
      printf("Process with rank [%d], sent %d to [%d].\n", rank, v[i],
              rank + 1);
    }

    for (int i = 0; i < nProcesses - 1; i++) {
      MPI_Recv(&value, 1, MPI_INT, MPI_ANY_SOURCE, 1, MPI_COMM_WORLD, &status);
      printf("Process with rank [%d], received %d from [%d].\n", rank, value,
             status.MPI_SOURCE);
      v[status.MPI_SOURCE - 1] = value;
    }
    displayVector(v, nProcesses - 1);
    compareVectors(v, vQSort, nProcesses - 1);
    free(v);
    free(vQSort);
  } else {
    // DONE: sort the vector v
    for (int i = 0; i < nProcesses - 1; i++) {
      int recvValue, sendValue;
      MPI_Recv(&recvValue, 1, MPI_INT, rank - 1, 1, MPI_COMM_WORLD, &status);
      printf("Process with rank [%d], received %d from [%d].\n", rank, recvValue,
              status.MPI_SOURCE);

      sendValue = recvValue;
      if (recvValue < value) {
        sendValue = value;
        value = recvValue;
      }

      if (rank != nProcesses - 1) {
        MPI_Send(&sendValue, 1, MPI_INT, rank + 1, 1, MPI_COMM_WORLD);
        printf("Process with rank [%d], sent %d to [%d].\n", rank, sendValue,
                rank + 1);
      }
    }

    printf("[%d] has %d.\n", rank, value);
    // Gathers the values from all the processes.
    MPI_Send(&value, 1, MPI_INT, MASTER, 1, MPI_COMM_WORLD);
    printf("Process with rank [%d], sent %d to [%d].\n", rank, value,
            MASTER);
  }

  MPI_Finalize();
  return 0;
}
