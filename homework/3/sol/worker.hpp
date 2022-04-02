#pragma once
#include "process.hpp"

class Worker : public Process {
public:
  Worker(int rank, int nr_of_processes);
  void start(void) override;

private:
  int my_coordinator;

  bool receive_topology(void);
  void receive_data(void);
  void process_data(void);
  void send_data(void);
  void work_with_data(void);
};