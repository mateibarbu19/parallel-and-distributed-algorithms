#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CHUCK_SIZE 5 // chunk dimension
#define ROOT 0

void *alloc_vector(size_t nmemb, size_t size) {
  void *v = calloc(nmemb, size);
  if (v == NULL) {
    fprintf(stderr, "alloc_vector: vector calloc failed!");
  }
  return v;
  return 0;
}
int main(int argc, char *argv[]) {
  int numtasks, rank, len;
  char hostname[MPI_MAX_PROCESSOR_NAME];

  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Get_processor_name(hostname, &len);

  int num_elements = CHUCK_SIZE * numtasks; // total elements
  int *v_send = NULL;
  int *v_recv =
      (int *)alloc_vector(CHUCK_SIZE, sizeof(*v_recv)); // partial vector

  // ROOT process generates the values for the full vector.
  if (rank == ROOT) {
    v_send = (int *)alloc_vector(num_elements, sizeof(*v_send)); // full vector
    memset(v_send, 0, num_elements * sizeof(*v_send));
  }

  // Scatter the vector to all processes.
  MPI_Scatter(v_send, CHUCK_SIZE, MPI_INT, v_recv, CHUCK_SIZE, MPI_INT, ROOT,
              MPI_COMM_WORLD);

  // Each process increments the values of the partial vector received.
  for (int i = 0; i < CHUCK_SIZE; i++) {
    v_recv[i] += rank;
  }

  // Gathers the values from all the processes.
  MPI_Gather(v_recv, CHUCK_SIZE, MPI_INT, v_send, CHUCK_SIZE, MPI_INT, ROOT,
             MPI_COMM_WORLD);

  // The ROOT process prints the elements received.
  if (rank == ROOT) {
    for (int i = 0; i < num_elements; i++) {
      printf("%d ", v_send[i]);
    }
    printf("\n");
  }

  if (rank == ROOT) {
    free(v_send);
  }
  free(v_recv);

  MPI_Finalize();
  return 0;
}