#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>

// #define MASTER 0
// changed master to be the last process

int main(int argc, char *argv[]) {
  int procs, rank;

  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &procs);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  int value = rank;
  int buffer;
  MPI_Status status;

  // Works for any number of processes
  for (int p = 1; p < procs; p <<= 1) {
    if (rank < procs - p) {
      MPI_Send(&value, 1, MPI_INT, rank + p, 1, MPI_COMM_WORLD);
      printf("Process with rank [%d], sent %d to [%d].\n", rank, value,
             rank + p);
    }
    if (rank >= p) {
      MPI_Recv(&buffer, 1, MPI_INT, rank - p, 1, MPI_COMM_WORLD, &status);
      printf("Process with rank [%d], received %d from [%d].\n", rank, buffer,
             status.MPI_SOURCE);
      value += buffer;
    }
  }

  if (rank == procs - 1) {
    printf("Result = %d\n", value);
  }

  MPI_Finalize();
}
