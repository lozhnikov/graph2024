#ifndef INCLUDE_BRIDGE_FINDER_HPP_
#define INCLUDE_BRIDGE_FINDER_HPP_

#include <algorithm>
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include "graph.hpp"
#include "iterators.hpp"

namespace graph {

void DfsHelper(size_t v, std::unordered_map<size_t, bool>& used,
                graph::Graph& gr, std::unordered_map<size_t, size_t>& tIn,
                std::unordered_map<size_t, size_t>& fUp, size_t& dfsTimer,
                std::unordered_map<size_t, size_t>& result,
                size_t p = -1) {
  used[v] = true;  // вершина num_peak посещена
  fUp[v] = tIn[v] =
      dfsTimer++;  // минимальное время поднятия из потомков num_peak
  std::unordered_set<size_t> neighbours =
      gr.IncomingEdges(v);  // все вершины смежныек с num_peak

  for (auto to : neighbours) {  // проходим все вершины связанные с num_peak
    if (to == p)
      continue;
    if (used[to]) {
      fUp[v] = std::min(fUp[v], tIn[to]);
    } else {
      DfsHelper(to, used, gr, tIn, fUp, dfsTimer, result, v);
      fUp[v] = std::min(fUp[v], fUp[to]);
      if (fUp[to] > tIn[v]) {
        result[to] = v;
        result[v] = to;
      }
    }
  }
}

std::unordered_map<size_t, size_t> BridgeFinder(graph::Graph& gr) {
  std::unordered_map<size_t, bool> vertices;
  std::unordered_map<size_t, size_t> timeIn;
  std::unordered_map<size_t, size_t> timeUp;
  std::unordered_map<size_t, size_t> result;
  VerticesRange tops = gr.Vertices();
  for (auto v : tops) {
    // false - вершина не была посещена
    // true - вершина была посещена
    vertices[v] = false;
  }
  size_t dfsTimer = 0;
  for (auto& [keyVertex, isUsed] : vertices) {
    if (!isUsed) {
      DfsHelper(keyVertex, vertices, gr, timeIn, timeUp, dfsTimer, result);
    }
  }
    return result;
}

}  // namespace graph

#endif  // INCLUDE_BRIDGE_FINDER_HPP_
