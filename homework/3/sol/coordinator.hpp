#pragma once
#include "process.hpp"
#include "utils.hpp"
#include <list>
#include <map>
#include <vector>

class Coordinator : public Process {
public:
  static const int ROOT = 0;
  static const int NR_OF_COORDINATORS = 3;
  Coordinator(int rank, int nr_of_processes, unsigned int array_size);
  void start(void) override;

private:
  unsigned int array_size;
  void read_workers(void);
  void send_worker_list(void);
  void receive_worker_lists(void);
  void transmit_topology_to_workers(void);
  void establish_topology(void);
  void distribute_data(void);
  void collect_data(void);
  void manage_data(void);
};