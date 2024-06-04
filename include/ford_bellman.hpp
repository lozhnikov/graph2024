/**
 * @file include/ford_bellman.hpp
 * @author Yaroslav Egorov
 *
 * Алгоритм Форда-Беллмана нахождения кратчайших путей от заданной вершины до всех остальных вершин
 */

#ifndef INCLUDE_FORD_BELLMAN_HPP_
#define INCLUDE_FORD_BELLMAN_HPP_

#include <vector>
#include <map>
#include <cstdint>
#include <utility>
#include <algorithm>
#include <iostream>
#include "weighted_oriented_graph.hpp"


namespace graph {

/**
 * @brief Алгоритм Форда-Беллмана
 */
template<typename WeightType>
struct edge {
  size_t begin;
  size_t end;
  WeightType weight;
};

template<typename WeightType>

/**
 * @brief Вспомогательна функция для алгоритма Форда-Беллмана
 */
int FordBellmanHelper(const graph::WeightedOrientedGraph<WeightType>&
graph) {
  int m = 0;
  for (size_t j : graph.Vertices()) {
      for (size_t neighbour : graph.Edges(j)) {
      ++m;
     }
  }
  return m;
}

/**
 * @brief Алгоритм Форда-Беллмана
 */
template<typename WeightType>
void FordBellman(const graph::WeightedOrientedGraph<WeightType>& graph,
std::map<size_t, WeightType>* res) {
  int m = FordBellmanHelper(graph);
  std::vector<edge<WeightType>> e(m);
  m = 0;

  for (size_t j : graph.Vertices()) {
    for (size_t neighbour : graph.Edges(j)) {
      e[m].begin = j;
      e[m].end = neighbour;
      e[m].weight = graph.EdgeWeight(j, neighbour);
      ++m;
    }
  }
  std::map<size_t, WeightType> d;
  for (size_t j : graph.Vertices()) {
    d[j] = 100000000;
  }
  d[0] = WeightType();

  while (true) {
    bool any = false;
    for (int j = 0; j < m; ++j) {
      if (d[e[j].begin] < 100000000) {
        if (d[e[j].end] > d[e[j].begin] + e[j].weight) {
          d[e[j].end] = d[e[j].begin] + e[j].weight;
          any = true;
        }
      }
    }
    if (!any)  break;
  }
  for (size_t i : graph.Vertices()) {
    res->insert(std::make_pair(i, d[i]));
  }
}

}  // namespace graph
#endif  // INCLUDE_FORD_BELLMAN_HPP_

