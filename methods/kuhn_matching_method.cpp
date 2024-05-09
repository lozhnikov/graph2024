#include <nlohmann/json.hpp>
#include <graph.hpp>
#include <oriented_graph.hpp>
#include <weighted_graph.hpp>
#include <weighted_oriented_graph.hpp>
#include <kuhn_matching.hpp>

using graph::Graph;
using graph::OrientedGraph;
using graph::WeightedGraph;
using graph::WeightedOrientedGraph;

namespace graph {

  template<typename GraphType>
  int KuhnMatchingMethodHelper(const nlohmann::json& input,
  nlohmann::json* output);

  int KuhnMatchingMethod(const nlohmann::json& input, nlohmann::json* output) {
    std::string graphType = input.at("graph_type");

    if (graphType == "Graph")
      return KuhnMatchingMethodHelper<Graph>(input, output);
    else if (graphType == "OrientedGraph")
      return KuhnMatchingMethodHelper<OrientedGraph>(input, output);
    else if (graphType == "WeightedGraph") {
      std::string weightType = input.at("weight_type");
      if (weightType == "int")
        return KuhnMatchingMethodHelper<WeightedGraph<int>>(input, output);
      else
        return -1;
    }
    else if (graphType == "WeightedOrientedGraph") {
      std::string weightType = input.at("weight_type");
      if (weightType == "int")
        return KuhnMatchingMethodHelper<WeightedOrientedGraph<int>>(input,
            output);
      else
        return -1;
    }

    return -1;
  }

  template<typename GraphType>
  int KuhnMatchingMethodHelper(const nlohmann::json& input,
  nlohmann::json* output) {
    GraphType graph;
    std::vector<std::pair<size_t, size_t>> result;

    for (auto& vertex : input.at("vertices")) {
      graph.AddVertex(vertex);
    }

    for (auto& edge : input.at("edges")) {
      graph.AddEdge(edge.at("from"), edge.at("to"));
    }

    KuhnMatching(graph, &result);

    for (size_t i = 0; i < result.size(); ++i) {
        (*output)["result"].push_back({ result[i].first, result[i].second });
    }

    return 0;
  }



}