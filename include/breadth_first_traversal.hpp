/**
 * @file include/breadth_first_traversal.hpp
 * @author Mikhail Lozhnikov
 *
 * Алгоритм обхода графа в ширину.
 */

#ifndef INCLUDE_BREADTH_FIRST_TRAVERSAL_HPP_
#define INCLUDE_BREADTH_FIRST_TRAVERSAL_HPP_

#include <unordered_set>
#include <queue>

namespace graph {

/**
 * @brief Вспомогательна функция для обхода графа в ширину.
 */
template<typename GraphType, typename Predicate>
void BreadthFirstTraversalHelper(GraphType& graph, Predicate predicate,
    size_t id, std::unordered_set<size_t>& used) {

  std::queue<size_t> ids;

  ids.push(id);

  used.insert(id);

  while (!ids.empty()) {
    size_t current = ids.front();
    ids.pop();

    predicate(current);

    for (size_t neighbour : graph.Edges(current)) {
      if (used.find(neighbour) == used.end())
        ids.push(neighbour);

      used.insert(neighbour);
    }
  }
}


/**
 * @brief Обход графа в ширину.
 */
template<typename GraphType, typename Predicate>
void BreadthFirstTraversal(GraphType& graph, Predicate predicate) {
  std::unordered_set<size_t> used;

  for (size_t id : graph.Vertices()) {
    if (used.find(id) == used.end()) {
      BreadthFirstTraversalHelper(graph, predicate, id, used);
    }
  }
}

}  // namespace graph

#endif  // INCLUDE_BREADTH_FIRST_TRAVERSAL_HPP_
