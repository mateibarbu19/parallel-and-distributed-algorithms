#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>

#define MASTER 0

int main (int argc, char *argv[])
{
    int procs, rank;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &procs);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int value;
    MPI_Status status;

    if (rank == MASTER) {
        value = 7;
    }

    for (int p = 1; p < procs; p <<= 1) {
        if (rank < p && rank + p < procs) {
            MPI_Send(&value, 1, MPI_INT, rank + p, 1, MPI_COMM_WORLD);
            printf("Process with rank [%d], sent %d to [%d].\n",
                rank, value, rank + p);
        }
        else if (rank >= p && rank < (p << 1)) {
            MPI_Recv(&value, 1, MPI_INT, rank - p, 1, MPI_COMM_WORLD, &status);
            printf("Process with rank [%d], received %d from [%d].\n",
                rank, value, status.MPI_SOURCE);
        }
    }

    printf("Process [%d] has value = %d\n", rank, value);

    MPI_Finalize();

}

