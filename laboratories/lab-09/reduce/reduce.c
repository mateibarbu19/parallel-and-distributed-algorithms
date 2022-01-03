#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>

#define MASTER 0

int main(int argc, char *argv[]) {
  int procs, rank;

  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &procs);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  int value = rank;
  int buffer;
  MPI_Status status;

  /* Works only for powers of 2
  for (int i = 2; i <= procs; i <<= 1) {
      if (rank % i == 0) {
          MPI_Recv(&buffer, 1, MPI_INT, rank + (i >> 1), 1, MPI_COMM_WORLD,
  &status); printf("Process with rank [%d], received %d from [%d].\n", rank,
  buffer, rank + (i >> 1)); value += buffer; } else if (rank % (i >> 1) == 0) {
          MPI_Send(&value, 1, MPI_INT, rank - (i >> 1), 1, MPI_COMM_WORLD);
          printf("Process with rank [%d], sent %d to [%d].\n",
              rank, value, rank - (i >> 1));
      }
  }
  */

  // Works for any number of processes
  for (int p = 1; p < procs; p <<= 1) {
    if (rank >= p) {
      MPI_Send(&value, 1, MPI_INT, rank - p, 1, MPI_COMM_WORLD);
      printf("Process with rank [%d], sent %d to [%d].\n", rank, value,
             rank - p);
    }
    if (rank < procs - p) {
      MPI_Recv(&buffer, 1, MPI_INT, rank + p, 1, MPI_COMM_WORLD, &status);
      printf("Process with rank [%d], received %d from [%d].\n", rank, buffer,
             status.MPI_SOURCE);
      value += buffer;
    }
  }

  if (rank == MASTER) {
    printf("Result = %d\n", value);
  }

  MPI_Finalize();
}
