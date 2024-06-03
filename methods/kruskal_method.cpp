#include <iostream>
#include <vector>
#include <utility>
#include <random>
#include <algorithm>
#include <string>
#include <kruskal.hpp>
#include <nlohmann/json.hpp>
#include <weighted_graph.hpp>

namespace graph {

  using nlohmann::json;
  using std::vector;
  using std::pair;
  using std::string;

  template<typename Weight>
  int KruskalMethodHelper(const json& input, json* output) {
    graph::WeightedGraph<Weight> g;
    for (auto vertex : input.at("vertices")) {
        g.AddVertex(vertex);
    }
    for (auto edge : input.at("edges")) {
        g.AddEdge(edge.at("from"), edge.at("to"), edge.at("weight"));
    }
    vector<pair<int, int>> t = Kruskal<Weight>(g);
    int i = 0;
    for (auto e : t) {
      (*output)["edges"][i] = e;
      i++;
    }
    return 1;
  }

  int KruskalMethod(const json& input, json* output) {
    string weightType = input.at("weight_type");
      if (weightType == "int") {
          return KruskalMethodHelper<int>(input, output);
      } else if (weightType == "double") {
          return KruskalMethodHelper<double>(input, output);
      } else {
          return -1;
      }
  }

}  //  namespace graph













