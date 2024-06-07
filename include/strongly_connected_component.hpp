/**
 * @file include/strongly_connected_component.hpp
 * @author Igor Bychkov
 *
 * Алгоритм нахождения сильной компоненты связности
*/
#ifndef INCLUDE_STRONGLY_CONNECTED_COMPONENT_HPP_
#define INCLUDE_STRONGLY_CONNECTED_COMPONENT_HPP_

#include <vector>
#include <unordered_map>
#include <map>
#include <list>
#include <utility>
#include <algorithm>
#include <iostream>
#include "oriented_graph.hpp"

namespace graph {

//template<typename OrientedGraph>

void DeepSearch1 (size_t v, const graph::OrientedGraph& g, 
std::map<size_t, int>* used, std::vector<size_t>* order) {
  (*used)[v] = 1;
  for (const auto& value : g.Edges(v)) {
    if(!(*used)[value]) {
      DeepSearch1 (value, g, used, order);
    }
    (*order).push_back(value);
  } 
}

void DeepSearch2 (size_t v, const graph::OrientedGraph& gr, 
std::map<size_t, int>* used, std::vector<size_t>* component) {
  (*used)[v] = 1;
  (*component).push_back(v);
  for (const auto& value : gr.Edges(v)) {
    if(!(*used)[value]) {
      DeepSearch2 (value, gr, used, component);
    }
  } 
}

/**
 * @brief Алгоритм Strong Connected Component
 */

void StronglyConnectedComponent(const graph::OrientedGraph& g, std::map<size_t,std::vector<size_t>>* result) 
{ //make transported graph
  graph::OrientedGraph gr;
  std::map<size_t, int> used;
  std::vector<size_t> component;
  std::vector<size_t> order;
  for (size_t j : g.Vertices()) {
    gr.AddVertex(j);
    for (size_t neighbour : g.Edges(j)) {
      if(g.HasEdge(j,neighbour))
        gr.AddEdge(neighbour, j);
    }
  }
  //used.assign (n, false);
  for (auto value : g.Vertices()) {
    if (!used[value]){  
      DeepSearch1(value, g, &used, &order);
    }
  }
  //used.assign (n, false);
  for (auto value : g.Vertices()) {
    size_t v = order.rbegin()[value]; //правильно ли?
    if (!(used[v])){  
      DeepSearch2(v, gr, &used, &component);
      size_t i = 0;
      for (auto var : component) {
        (*result)[i].push_back(var);
        ++i;
        }
      component.clear();
    }
  }  
}

}  // namespace graph
#endif