#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#define ROOT 0
// How many numbers will be sent.
#define NR_LOOPS 10

int main(int argc, char *argv[]) {
  int numtasks, rank;

  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  // Checks the number of processes allowed.
  if (numtasks != 2) {
    printf("Wrong number of processes. Only 2 allowed!\n");
    MPI_Finalize();
    return 1;
  }

  int value, tag;
  MPI_Status status;

  if (rank == 0) {
    // Receives the information from the first process.
    for (int i = 0; i < NR_LOOPS; i++) {
      MPI_Recv(&value, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD,
               &status);
      // Prints the numbers with their corresponding tags.
      printf("Process [%d] recieved %d with tag %d.\n", rank, value,
             status.MPI_TAG);
    }
  } else {
    for (int i = 0; i < NR_LOOPS; i++) {
      // Generate the random numbers.
      struct timeval t;
      gettimeofday(&t, NULL);
      srand(t.tv_usec);
      value = rand() % (rank * 50 + 1);
      // Generate the random tags.
      tag = rand() % (rank * 50 + 1);

      // Sends the numbers with the tags to the second process.
      MPI_Send(&value, 1, MPI_INT, ROOT, 0, MPI_COMM_WORLD);
      printf("Process [%d] sent %d with tag %d.\n", rank, value, tag);
    }
  }

  MPI_Finalize();
  return 0;
}