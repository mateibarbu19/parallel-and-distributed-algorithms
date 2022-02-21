#pragma once
#include <list>
#include <map>
#include <string>

#define WORKER_LIST_TAG 1
#define TOPOLOGY_TAG 2
#define COORDINATOR_DATA 3
#define WORKER_DATA 4

std::string to_workers_id_string(const std::list<int> &workers_list);

std::string to_topology_string(std::map<int, std::list<int>> topology);
