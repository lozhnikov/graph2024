/**
 * @file include/my_algorithm_prins.hpp
 * @author Bogdan Kushnarenko
 *
 * Алгоритм Прима для нахождения минимального остова в разреженном графе
*/

#ifndef INCLUDE_MY_ALGORITHM_PRIMS_HPP_
#define INCLUDE_MY_ALGORITHM_PRIMS_HPP_

#include <iostream>
#include <vector>
#include <unordered_map>
#include <set>
#include <utility>

namespace graph {

/**
 * @brief Алгоритм Прима
 */
template <typename WeightType>
void MyAlgorithmPrims(const graph::WeightedGraph<WeightType>& graph,
std::vector<std::pair<size_t, size_t>>* resEdges) {
    std::vector<std::pair<size_t, size_t>> edges(graph.NumVertices() - 1);
    std::unordered_map<size_t, WeightType> minEdge;
    std::unordered_map<size_t, size_t> selEdge;
    std::set<std::pair<size_t, size_t>> q;
    size_t vertexBegin = *(graph.Vertices().begin());
    minEdge[vertexBegin] = WeightType();
    q.insert(std::make_pair(vertexBegin, vertexBegin));

    int iterator = 0;
    for (size_t i = 0; i < graph.NumVertices(); ++i) {
        if (q.empty()) {
          std::cout << "No MST!";
          exit(0);
        }
        size_t v = q.begin()->second;
        q.erase(q.begin());

        if (selEdge.find(v) != selEdge.end()) {
          edges[iterator].first = selEdge.at(v);
          edges[iterator].second = v;
          ++iterator;
        }

        for (const auto& elem : graph.Edges(v)) {
          size_t to = elem;
          WeightType cost = graph.EdgeWeight(v, elem);
          if (minEdge.find(to) == minEdge.end() || cost < minEdge.at(to)) {
              q.erase(std::make_pair(minEdge[to], to));
              minEdge[to] = cost;
              selEdge[to] = v;
              q.insert(std::make_pair(cost, to));
            }
        }
    }

    *resEdges = edges;
}

}  // namespace graph


#endif  // INCLUDE_MY_ALGORITHM_PRIMS_HPP_
