/**
 * @file include/kuhn_matching.hpp
 * @author Samarokov Aleksei
 *
 * Алгоритм Куна нахождения наибольшего паросочетания в двудольном графе.
 */


 //#ifndef KUHN_MATCHING_HPP
 //#define KUHN_MATCHING_HPP
 //
 //#include "graph.hpp"
 //#include <vector>
 //
 //namespace graph {
 //  template<typename GraphType>
 //  class KuhnMatching {
 //  public:
 //    KuhnMatching(const GraphType& graph) : graph(graph) {}
 //
 //    bool FindMatching() {
 //      for (const auto& vertex : graph.Vertices()) {
 //        used.assign(graph.NumVertices(), false);
 //        if (TryKuhn(vertex)) {
 //        ++matching_size;
 //        }
 //      }
 //      return matchingSize > 0;
 //    }
 //
 //    std::vector<std::pair<size_t, size_t>> GetMatching() const {
 //      std::vector<std::pair<size_t, size_t>> result;
 //      for (size_t i = 0; i < graph.NumVertices(); ++i) {
 //        if (matching[i] != -1) {
 //        result.emplace_back(matching[i], i);
 //        }
 //      }
 //      return result;
 //    }
 //
 //  private:
 //    bool TryKuhn(size_t v) {
 //      if (used[v]) {
 //        return false;
 //      }
 //      used[v] = true;
 //      for (size_t u : graph.Edges(v)) {
 //        if (matching[u] == -1 || TryKuhn(matching[u])) {
 //        matching[u] = v;
 //        return true;
 //        }
 //      }
 //      return false;
 //    }
 //
 //    const GraphType& graph;
 //    std::vector<bool> used;
 //    std::vector<int> matching;
 //    size_t matchingSize = 0;
 //  };
 //
 //} // namespace graph
 //
 //#endif // KUHN_MATCHING_HPP

 //
 //#ifndef KUHN_MATCHING_HPP
 //#define KUHN_MATCHING_HPP
 //
 //#include "graph.hpp"
 //#include <vector>
 //
 //namespace graph {
 //
 //template<typename GraphType>
 //const GraphType& graph;
 //std::vector<int> matching;
 //std::vector<bool> used;
 //size_t matching_size = 0;
 //
 //
 //auto TryKuhn = [&](size_t v) -> bool {
 //    if (used[v]) {
 //        return false;
 //    }
 //    used[v] = true;
 //    for (size_t u : graph.Edges(v)) {
 //        if (matching[u] == -1 || TryKuhn(matching[u])) {
 //            matching[u] = v;
 //            return true;
 //        }
 //    }
 //    return false;
 //    };
 //
 //
 //std::vector<std::pair<size_t, size_t>> KuhnMatching(const Graph& graph) {
 //    matching.assign(graph.NumVertices(), -1);
 //    used.assign(graph.NumVertices(), false);
 //
 //    auto FindMatching = [&]() -> bool {
 //        for (const auto& vertex : graph.Vertices()) {
 //            used.assign(graph.NumVertices(), false);
 //            if (TryKuhn(vertex)) {
 //                ++matching_size;
 //            }
 //        }
 //        return matching_size > 0;
 //    };
 //
 //    std::vector<std::pair<size_t, size_t>> result;
 //    if (FindMatching()) {
 //        for (size_t i = 0; i < matching.size(); ++i) {
 //            if (matching[i] != -1) {
 //                result.emplace_back(matching[i], i);
 //            }
 //        }
 //    }
 //
 //    return result;
 //}
 //
 //} // namespace graph
 //#endif
 //
 //





#include "graph.hpp"
#include <vector>

namespace graph {
    template<typename GraphType>
    class Kuhn {
    public:
        Kuhn(GraphType& graph) : graph_(graph) {}

        bool FindMatching() {
            for (const auto& vertex : graph_.Vertices()) {
                used.assign(graph_.NumVertices(), false);
                if (TryKuhn(vertex)) {
                    ++matching_size;
                }
            }
            return matching_size > 0;
        }

        std::vector<std::pair<size_t, size_t>> GetMatching() const {
            std::vector<std::pair<size_t, size_t>> result;
            for (size_t i = 0; i < graph_.NumVertices(); ++i) {
                if (matching[i] != -1) {
                    result.emplace_back(matching[i], i);
                }
            }
            return result;
        }

    private:
        bool TryKuhn(size_t v) {
            if (used[v]) {
                return false;
            }
            used[v] = true;
            for (size_t u : graph_.Edges(v)) {
                if (matching[u] == -1 || TryKuhn(matching[u])) {
                    matching[u] = v;
                    return true;
                }
            }
            return false;
        }

        GraphType& graph_;
        std::vector<bool> used;
        std::vector<int> matching;
        size_t matching_size = 0;
    };

    template<typename GraphType>
    void KuhnMatching(GraphType& graph,
        std::vector<std::pair<size_t, size_t>>* resEdges) {
        Kuhn result(graph);
        *resEdges = result.GetMatching();
    };

} // namespace graph
