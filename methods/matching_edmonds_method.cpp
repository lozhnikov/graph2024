#include <nlohmann/json.hpp>
#include <graph.hpp>
#include <oriented_graph.hpp>
#include <weighted_graph.hpp>
#include <weighted_oriented_graph.hpp>
#include <matching_edmonds.hpp>

using graph::Graph;
using graph::OrientedGraph;
using graph::WeightedGraph;
using graph::WeightedOrientedGraph;

namespace graph {

template<typename Graph>
int EdmondsMatchingMethodHelper(const nlohmann::json& input,
nlohmann::json* output);

int EdmondsMatchingMethod(const nlohmann::json& input, nlohmann::json* output) {
  std::string graphType = input.at("graph_type");

  if (graphType == "Graph") {
    return EdmondsMatchingMethodHelper<Graph>(input, output);
  } else if (graphType == "OrientedGraph") {
    return EdmondsMatchingMethodHelper<OrientedGraph>(input, output);
  } else if (graphType == "WeightedGraph") {
    std::string weightType = input.at("weight_type");
    if (weightType == "int") {
      return EdmondsMatchingMethodHelper<WeightedGraph<int>>(input, output);
    } else {
      return -1;
    }
  } else if (graphType == "WeightedOrientedGraph") {
    std::string weightType = input.at("weight_type");
    if (weightType == "int") {
      return EdmondsMatchingMethodHelper<WeightedOrientedGraph<int>>(input,
      output);
    } else {
      return -1;
    }
  }

  return -1;
}

template<typename Graph>
int EdmondsMatchingMethodHelper(const nlohmann::json& input,
nlohmann::json* output) {
  Graph graph;
  for (auto vert : input["vertices"]) {
    graph.AddVertex(vert);
  }
  for (auto edge : input["edges"]) {
    graph.AddEdge(edge[0], edge[1]);
  }
  (*output) = {{"greatest matching", MatchingEdmonds(graph)}};

  return 0;
}
}  // namespace graph
