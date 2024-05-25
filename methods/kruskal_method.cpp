#include <kruskal.hpp>
#include <nlohmann/json.hpp>
#include <iostream>
#include <vector>
#include <utility>
#include <random>
#include <algorithm>

namespace graph {

  using nlohmann::json;
  using std::vector;
  using std::pair;

  int KruskalMethod(const json& input, json* output)
  {
    vector<pair<int,pair<int,int>>> g;

    for (auto& elem : input.at("graph")) {
      pair<int,pair<int,int>> p;
      p.first = elem[0];
      p.second.first = elem[1][0];
      p.second.second = elem[1][1];
      g.push_back(p);
    }

    vector<pair<int,int>> t = Kruskal(g, input["numVertices"]);

    int i = 0;
    for (pair<int,int> e : t) {
      (*output)["edges"][i] = e;
      i++;
    }

    return 1;
  }

}













