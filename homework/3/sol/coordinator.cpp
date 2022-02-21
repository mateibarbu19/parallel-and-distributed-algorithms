#include "coordinator.hpp"
#include "process.hpp"
#include "utils.hpp"
#include <cstdio>
#include <mpi/mpi.h>
#include <numeric>
#include <string>
#include <vector>

Coordinator::Coordinator(int rank, int nr_of_processes, unsigned int array_size)
    : Process(rank, nr_of_processes) {
  this->array_size = array_size;
}

void Coordinator::read_workers() {
  std::string f_name = "cluster" + std::to_string(rank) + ".txt";
  FILE *f_in = fopen(f_name.c_str(), "rt");
  topology.emplace(std::make_pair(rank, std::list<int>()));

  unsigned int my_workers;
  fscanf(f_in, "%u", &my_workers);
  for (unsigned int i = 0; i < my_workers; i++) {
    unsigned int worker_id;
    fscanf(f_in, "%i", &worker_id);
    topology[rank].push_back(worker_id);
  }

  fclose(f_in);
}

void Coordinator::send_worker_list(void) {
  std::string my_workers = to_workers_id_string(topology[rank]);
  int size = my_workers.size();

  // send my list of workers to all other coordinators
  for (int i = 0; i < NR_OF_COORDINATORS; i++) {
    if (i != rank) {
      MPI_Send(&size, 1, MPI_INT, i, WORKER_LIST_TAG, MPI_COMM_WORLD);
      printf("M(%d,%d)\n", rank, i);
      MPI_Send(my_workers.c_str(), size, MPI_CHAR, i, WORKER_LIST_TAG,
               MPI_COMM_WORLD);
      printf("M(%d,%d)\n", rank, i);
    }
  }
}

void Coordinator::receive_worker_lists(void) {
  // receive form all other coordinators their worker list
  MPI_Status status;
  for (int i = 0; i < NR_OF_COORDINATORS; i++) {
    if (i != rank) {
      int size;
      MPI_Recv(&size, 1, MPI_INT, i, WORKER_LIST_TAG, MPI_COMM_WORLD, &status);

      topology.emplace(std::make_pair(i, std::list<int>()));

      char *buff = new char[size + 1];
      MPI_Recv(buff, size, MPI_CHAR, i, WORKER_LIST_TAG, MPI_COMM_WORLD,
               &status);

      buff[size] = '\0';
      char *p = strtok(buff, ",");
      while (p != NULL) {
        topology[i].push_back(atoi(p));
        p = strtok(NULL, ",");
      }

      delete[] buff;
    }
  }
}

void Coordinator::transmit_topology_to_workers(void) {
  std::string topology_string = to_topology_string(topology);
  int size = topology_string.size();

  // send the topology to all my workers
  for (int worker_id : topology[rank]) {
    MPI_Send(&size, 1, MPI_INT, worker_id, TOPOLOGY_TAG, MPI_COMM_WORLD);
    printf("M(%d,%d)\n", rank, worker_id);
    MPI_Send(topology_string.c_str(), size, MPI_CHAR, worker_id, TOPOLOGY_TAG,
             MPI_COMM_WORLD);
    printf("M(%d,%d)\n", rank, worker_id);
  }
}

void Coordinator::establish_topology(void) {
  read_workers();

  send_worker_list();

  receive_worker_lists();

  transmit_topology_to_workers();

  print_topology();
}

void Coordinator::distribute_data(void) {
  // resize my internal buffer
  int dis, send_size;
  int nr_of_workers = nr_of_processes - NR_OF_COORDINATORS;
  int buffer_size = (array_size / nr_of_workers) * topology[rank].size();
  if (rank == NR_OF_COORDINATORS - 1) {
    buffer_size = array_size - (array_size / nr_of_workers) *
                                   (nr_of_workers - topology[rank].size());
  }
  buffer.resize(buffer_size);

  // distribute the initial data among coordinators
  if (rank == ROOT) {
    // set up the intial data
    std::vector<int> raw(array_size);
    std::iota(std::begin(raw), std::end(raw), 0);

    dis = 0; // displacement
    for (int i = 0; i < NR_OF_COORDINATORS; i++) {
      send_size = (array_size / nr_of_workers) * topology[i].size();
      if (i == NR_OF_COORDINATORS - 1) {
        send_size = array_size - (array_size / nr_of_workers) *
                                     (nr_of_workers - topology[i].size());
      }

      if (i != ROOT) {
        MPI_Send(raw.data() + dis, send_size, MPI_INT, i, COORDINATOR_DATA,
                 MPI_COMM_WORLD);
        printf("M(%d,%d)\n", rank, i);
      } else {
        copy(raw.begin() + dis, raw.begin() + dis + send_size, buffer.begin());
      }

      dis += send_size;
    }
  } else {
    // receive data from ROOT
    MPI_Status status;
    MPI_Recv(buffer.data(), buffer.size(), MPI_INT, ROOT, COORDINATOR_DATA,
             MPI_COMM_WORLD, &status);
  }

  // scatter data among workers
  int i = 0;
  for (int worker_id : topology[rank]) {
    dis = buffer.size() * i / topology[rank].size();
    send_size = buffer.size() * (i + 1) / topology[rank].size() - dis;

    MPI_Send(&send_size, 1, MPI_INT, worker_id, WORKER_DATA, MPI_COMM_WORLD);
    printf("M(%d,%d)\n", rank, worker_id);

    MPI_Send(buffer.data() + dis, send_size, MPI_INT, worker_id, WORKER_DATA,
             MPI_COMM_WORLD);
    printf("M(%d,%d)\n", rank, worker_id);

    i++;
  }
}

void Coordinator::collect_data(void) {
  MPI_Status status;
  int dis, recv_size;

  // gather data from workers
  int i = 0;
  for (int worker_id : topology[rank]) {
    dis = buffer.size() * i / topology[rank].size();
    recv_size = buffer.size() * (i + 1) / topology[rank].size() - dis;

    MPI_Recv(buffer.data() + dis, recv_size, MPI_INT, worker_id, WORKER_DATA,
             MPI_COMM_WORLD, &status);

    i++;
  }

  if (rank != ROOT) {
    // give gathered data to ROOT
    MPI_Send(buffer.data(), buffer.size(), MPI_INT, ROOT, COORDINATOR_DATA,
             MPI_COMM_WORLD);
    printf("M(%d,%d)\n", rank, ROOT);
  } else if (rank == ROOT) {
    // collect the final gata
    std::vector<int> processed(array_size);

    dis = 0; // displacement
    int nr_of_workers = nr_of_processes - NR_OF_COORDINATORS;
    for (int i = 0; i < NR_OF_COORDINATORS; i++) {
      recv_size = (array_size / nr_of_workers) * topology[i].size();
      if (i == NR_OF_COORDINATORS - 1) {
        recv_size = array_size - (array_size / nr_of_workers) *
                                     (nr_of_workers - topology[i].size());
      }

      if (i != ROOT) {
        MPI_Recv(processed.data() + dis, recv_size, MPI_INT, i,
                 COORDINATOR_DATA, MPI_COMM_WORLD, &status);
      } else {
        copy(buffer.begin(), buffer.end(), processed.begin() + dis);
      }

      dis += recv_size;
    }

    printf("Rezultat: ");
    for (int r : processed) {
      printf("%d ", r);
    }
    puts("");
  }
}

void Coordinator::manage_data(void) {
  distribute_data();

  collect_data();
}

void Coordinator::start(void) {
  establish_topology();
  manage_data();
}