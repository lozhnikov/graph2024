/**
 * @file include/my_algorithm_prins.hpp
 * @author Yaroslav Egorov
 *
 * Алгоритм Форда-Беллмана нахождения кратчайших путей от заданной вершины до всех остальных вершин
 */

#ifndef INCLUDE_FORD_BELLMAN_HPP_
#define INCLUDE_FORD_BELLMAN_HPP_

#include <vector>
#include <algorithm>
#include <iostream>
#include "weighted_oriented_graph.hpp"
#define INF 10000000


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
std::vector<WeightType>* res) {
  int m = FordBellmanHelper(graph);
  std::vector<edge<WeightType>> e(m);
  res->resize(m);
  m = 0;
  for (size_t j : graph.Vertices()) {
    for (size_t neighbour : graph.Edges(j)) {
      e[m].begin = j;
      e[m].end = neighbour;
      e[m].weight = graph.EdgeWeight(j, neighbour);
      ++m;
    }
  }
  std::vector<WeightType> d(graph.NumVertices(), INF);
  d[0] = 0;
  std::vector<WeightType> p(graph.NumVertices(), -1);
  while (true) {
    bool any = false;
    for (int j = 0; j < m; ++j) {
      if (d[e[j].begin] < INF) {
        if (d[e[j].end] > d[e[j].begin] + e[j].weight) {
          d[e[j].end] = d[e[j].begin] + e[j].weight;
          p[e[j].end] = e[j].begin;
          any = true;
        }
      }
    }
    if (!any)  break;
  }
  for (int i = 0; i < graph.NumVertices(); i++) {
    res->at(i) = d[i];
  }
}

}  // namespace graph
#endif  // INCLUDE_FORD_BELLMAN_HPP_

