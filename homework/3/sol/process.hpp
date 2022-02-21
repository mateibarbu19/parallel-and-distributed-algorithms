#pragma once
#include <list>
#include <map>
#include <vector>

class Process {
public:
  virtual void start(void) = 0;
  Process(int rank, int nr_of_processes);
  virtual ~Process();

protected:
  int rank;
  int nr_of_processes;
  std::map<int, std::list<int>> topology;
  std::vector<int> buffer;

  void print_topology(void);
};