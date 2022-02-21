#include "utils.hpp"
#include <functional>
#include <numeric>
#include <string>

std::string to_workers_id_string(const std::list<int> &workers_list) {
  return std::accumulate(++workers_list.begin(), workers_list.end(),
                         std::to_string(*workers_list.begin()),
                         [](const std::string &a, const int &b) {
                           return a + ',' + std::to_string(b);
                         });
}

std::string to_topology_string(std::map<int, std::list<int>> topology) {
  return std::accumulate(
      topology.begin(), topology.end(), std::string(""),
      [](const std::string &a, std::pair<const int, std::list<int>> b) {
        return a + ' ' + std::to_string(b.first) + ':' +
               to_workers_id_string(b.second);
      });
}