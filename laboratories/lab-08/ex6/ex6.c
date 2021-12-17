#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#define GROUP_SIZE 4

int main(int argc, char *argv[]) {
  int old_size, new_size;
  int old_rank, new_rank;
  int recv_rank;
  MPI_Comm custom_group;

  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &old_size); // Total number of processes.
  MPI_Comm_rank(MPI_COMM_WORLD, &old_rank); // The current process ID / Rank.

  // Checks the number of processes allowed.
  if (old_size % GROUP_SIZE) {
    printf("Wrong number of processes. Only a multiple of %d allowed!\n",
           GROUP_SIZE);
    MPI_Finalize();
    return 1;
  }

  // Split the MPI_COMM_WORLD in small groups.
  MPI_Comm tribes;
  int color = old_rank / GROUP_SIZE;
  int rank = old_rank % GROUP_SIZE;
  MPI_Comm_split(MPI_COMM_WORLD, color, rank, &tribes);

  MPI_Comm_size(tribes, &new_size); // Total number of processes.
  MPI_Comm_rank(tribes, &new_rank); // The current process ID / Rank.
  printf("Rank [%d] / size [%d] in MPI_COMM_WORLD and rank [%d] / size [%d] in "
         "custom group.\n",
         old_rank, old_size, new_rank, new_size);

  // Send the rank to the next process.
  MPI_Send(&new_rank, 1, MPI_INT, (new_rank + 1) % new_size, 1, tribes);

  // Receive the rank.
  MPI_Status status;
  MPI_Recv(&recv_rank, 1, MPI_INT, (new_rank + new_size - 1) % GROUP_SIZE, 1, tribes, &status);

  printf("Process [%d] from group [%d] received [%d].\n", new_rank,
         color, recv_rank);

  MPI_Finalize();
  return 0;
}