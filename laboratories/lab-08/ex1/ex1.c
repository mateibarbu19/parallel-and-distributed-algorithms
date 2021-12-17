#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  int numtasks, rank;

  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  int random_num;
  MPI_Status status;

  // First process starts the circle.
  if (rank == 0) {
    // First process starts the circle.

    // Generate a random number.
    srand(42);
    random_num = rand();

    // Send the number to the next process.
    MPI_Send(&random_num, 1, MPI_INT, rank + 1, 1, MPI_COMM_WORLD);
    printf("Process with rank [%d] sent the number %d.\n", rank, random_num);

    MPI_Recv(&random_num, 1, MPI_INT, numtasks - 1, 1, MPI_COMM_WORLD, &status);
    printf("Process with rank [%d], received %d with tag %d.\n", rank,
           random_num, status.MPI_TAG);
  } else {
    // Regular process.

    // Receives the number from the previous process.
    MPI_Recv(&random_num, 1, MPI_INT, rank - 1, 1, MPI_COMM_WORLD, &status);
    printf("Process with rank [%d], received %d with tag %d.\n", rank,
           random_num, status.MPI_TAG);

    // Increments the number.
    random_num++;

    // Sends the number to the next process.
    MPI_Send(&random_num, 1, MPI_INT, (rank + 1) % numtasks, 1, MPI_COMM_WORLD);
    printf("Process with rank [%d] sent the number %d.\n", rank, random_num);
  }

  MPI_Finalize();
  return 0;
}