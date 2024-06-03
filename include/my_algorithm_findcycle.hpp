/**
 * @file include/my_algorithm_findcycle.hpp
 * @author Oleg Malashevich
 * 
 * Проверка графа на ацикличность
*/

#ifndef INCLUDE_MY_ALGORITHM_FINDCYCLE_HPP_
#define INCLUDE_MY_ALGORITHM_FINDCYCLE_HPP_

#include <iostream>
#include <list>
#include <map>
#include <oriented_graph.hpp>

namespace graph {

int dfs(size_t v, const graph::OrientedGraph& graph, 
std::map<size_t, int>* color, std::list<size_t>* cycle) {
  (*color)[v] = 1; //типа серый
  for (auto& to : graph.Edges(v)){
    if ((*color)[to] == 0) {
      (*cycle).push_back(to);
      if (dfs(to, graph, color, cycle)) { return 1; }
    }
    else if ((*color)[to] == 1) {
    (*cycle).push_back(to);
    return 1;
    }
  }
  (*color)[v] = 2; //типа чёрный
  (*cycle).pop_back(); //убираем ту вершину из конца списка
  return 0;
}

int MyAlgorithmFindCycle(const graph::OrientedGraph& graph, 
std::list<size_t>* cycle, std::map<size_t, int> color) {
  
  for (auto v : graph.Vertices()) {
    if (color[v] == 0){
      cycle->push_back(v);
      if (dfs(v, graph, &color, cycle)){
        size_t tmp = cycle->back();
        while (cycle->front() != tmp) { cycle->pop_front(); }
        return 1; 
      }
    }
  }
  
  return 0;
}
}

// 0 if no cycle, 1 if there is a cycle
#endif
