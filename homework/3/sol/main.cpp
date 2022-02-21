#include "coordinator.hpp"
#include "process.hpp"
#include "utils.hpp"
#include "worker.hpp"
#include <cstdio>
#include <cstdlib>
#include <mpi/mpi.h>

int main(int argc, char *argv[]) {
  int rank, nr_of_processes;
  unsigned int array_size;
  Process *process;

  MPI_Init(&argc, &argv);
  array_size = atoi(argv[1]);

  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &nr_of_processes);

  if (rank < Coordinator::NR_OF_COORDINATORS) {
    process = (Process *)new Coordinator(rank, nr_of_processes, array_size);
  } else {
    process = (Process *)new Worker(rank, nr_of_processes);
  }

  process->start();

  delete process;

  MPI_Finalize();
  return 0;
}
