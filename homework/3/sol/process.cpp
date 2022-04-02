#include "process.hpp"
#include "utils.hpp"
#include <cstdio>
#include <string>

Process::Process(int rank, int nr_of_processes) {
  this->rank = rank;
  this->nr_of_processes = nr_of_processes;
  topology.clear();
}

void Process::print_topology(void) {
  printf("%d ->", rank);
  std::string topology_string = to_topology_string(topology);
  puts(topology_string.c_str());
}

Process::~Process() {}