#ifndef INCLUDE_BRIDGE_FINDER_HPP_
#define INCLUDE_BRIDGE_FINDER_HPP_

#include <algorithm>
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include "graph.hpp"
#include "iterators.hpp"

namespace graph {

void Dfs_helper(size_t v, std::unordered_map<size_t, bool>& used,
                graph::Graph& Gr, std::unordered_map<size_t, size_t>& tin,
                std::unordered_map<size_t, size_t>& fup, size_t& dfs_timer, std::unordered_map<size_t,size_t>& result,
                size_t p = -1) {
  used[v] = true;  // вершина num_peak посещена
  fup[v] = tin[v] =
      dfs_timer++;  // минимальное время поднятия из потомков num_peak
  std::unordered_set<size_t> neighbours =
      Gr.IncomingEdges(v);  // все вершины смежныек с num_peak

  for (auto to : neighbours) {  // проходим все вершины связанные с num_peak
    if (to == p)
      continue;
    if (used[to])
      fup[v] = std::min(fup[v], tin[to]);
    else {
      Dfs_helper(to, used, Gr, tin, fup, dfs_timer,result, v);
      fup[v] = std::min(fup[v], fup[to]);
      if (fup[to] > tin[v]) {
        result[to] = v;
        result[v] = to; 
      }
    }
  }
}

std::unordered_map<size_t, size_t> bridge_finder(graph::Graph& Gr) {
  std::unordered_map<size_t, bool> vertices;
  std::unordered_map<size_t, size_t> time_in;
  std::unordered_map<size_t, size_t> time_up;
  std::unordered_map<size_t,size_t> result;
  VerticesRange tops = Gr.Vertices();
  for (auto v : tops) {
    // false - вершина не была посещена
    // true - вершина была посещена
    vertices[v] = false;
  }
  size_t dfs_timer = 0;
  for (auto& [key_vertex, is_used] : vertices) {
    if (!is_used) {
      Dfs_helper(key_vertex, vertices, Gr, time_in, time_up,dfs_timer, result);
    }
  }
    return result;
}

}  // namespace graph

#endif  // INCLUDE_BRIDGE_FINDER_HPP_и








