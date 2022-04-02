#include "worker.hpp"
#include "mpi/mpi.h"
#include "utils.hpp"
#include <algorithm>
#include <cstdio>

Worker::Worker(int rank, int nr_of_processes)
    : Process(rank, nr_of_processes) {}

bool Worker::receive_topology(void) {
  MPI_Status status;
  int size;
  MPI_Recv(&size, 1, MPI_INT, MPI_ANY_SOURCE, TOPOLOGY_TAG, MPI_COMM_WORLD,
           &status);

  char *buff = new char[size + 1];
  MPI_Recv(buff, size, MPI_CHAR, status.MPI_SOURCE, TOPOLOGY_TAG,
           MPI_COMM_WORLD, &status);

  buff[size] = '\0';
  char *p = strtok(buff, " ");
  while (p != NULL) {
    int coordinator_id, pos;

    sscanf(p, " %d:%n", &coordinator_id, &pos);
    p += pos;

    topology.emplace(std::make_pair(coordinator_id, std::list<int>()));

    do {
      int worker_id;
      sscanf(p, "%d%n", &worker_id, &pos);
      topology[coordinator_id].push_back(worker_id);
      p += pos;
    } while (*p++ == ',');
    p = strtok(NULL, " ");
  }

  delete[] buff;

  my_coordinator = status.MPI_SOURCE;
  std::list<int> &list = topology[my_coordinator];
  return std::find(list.begin(), list.end(), rank) != list.end();
}

void Worker::receive_data(void) {
  int recv_size;
  MPI_Status status;

  // get data from coordinator
  MPI_Recv(&recv_size, 1, MPI_INT, my_coordinator, WORKER_DATA, MPI_COMM_WORLD,
           &status);
  buffer.resize(recv_size);
  MPI_Recv(buffer.data(), buffer.size(), MPI_INT, my_coordinator, WORKER_DATA,
           MPI_COMM_WORLD, &status);
}

void Worker::process_data(void) {
  std::for_each(buffer.begin(), buffer.end(), [](int &v) { v *= 2; });
}

void Worker::send_data(void) {
  MPI_Send(buffer.data(), buffer.size(), MPI_INT, my_coordinator, WORKER_DATA,
           MPI_COMM_WORLD);
  printf("M(%d,%d)\n", rank, my_coordinator);
}

void Worker::work_with_data(void) {
  receive_data();

  process_data();

  send_data();
}

void Worker::start(void) {
  while (!receive_topology()) {
    topology.clear();
  }

  print_topology();

  work_with_data();
}