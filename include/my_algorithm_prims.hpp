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
#include <set>
#include <utility>

namespace graph {

/**
 * @brief Алгоритм Прима
 */
template<typename WeightType>
void MyAlgorithmPrims(const graph::WeightedGraph<WeightType>& graph,
std::vector<std::vector<WeightType>>* resEdges) {
    const int INF = 1000000000;
    std::vector<int> vertices(graph.NumVertices());
    std::vector<std::vector<WeightType>>
    edges(graph.NumVertices() - 1, std::vector<WeightType>(2));
    std::vector<int> minEdge(static_cast<int>(graph.NumVertices()), INF),
    selEdge(static_cast<int>(graph.NumVertices()), -1);
    minEdge[0] = 0;
    std::set < std::pair<int, int> > q;
    q.insert(std::make_pair(0, 0));

    int iterator = 0;
    for (size_t it = 0; it < graph.NumVertices(); ++it) {
      vertices[it] = static_cast<int>(it);
    }

    for (size_t i = 0; i < graph.NumVertices(); ++i) {
        if (q.empty()) {
          std::cout << "No MST!";
          exit(0);
        }
        int v = q.begin()->second;
        q.erase(q.begin());

        if (selEdge[v] != -1) {
          edges[iterator][0] = selEdge[v];
          edges[iterator][1] = v;
          ++iterator;
        }

        std::unordered_set<size_t> vertWithV = graph.Edges(v);

        for (const auto& elem : vertWithV) {
          int to = elem, cost = graph.EdgeWeight(v, elem);
          if (cost < minEdge[to]) {
              q.erase(std::make_pair(minEdge[to], to));
              minEdge[to] = cost;
              selEdge[to] = v;
              q.insert(std::make_pair(minEdge[to], to));
            }
        }
    }

    *resEdges = edges;
}

}  // namespace graph


#endif  // INCLUDE_MY_ALGORITHM_PRIMS_HPP_
