#include <nlohmann/json.hpp>
#include <oriented_graph.hpp>
#include <my_algorithm_findcycle.hpp>
#include <map>
#include<list>

namespace graph {
int MyAlgorithmFindCycleMethod(const nlohmann::json& input, nlohmann::json* output) {
  std::string GraphType = input.at("graph_type");
  
  if(GraphType != "type_oriented"){ return -1; }
  
  
  graph::OrientedGraph g;
  std::list<size_t> cycle;
  std::map<size_t, int> color; //множество, для задания цвета вершинам,
  
  for (auto& vertex : input.at("vertices")) { 
    g.AddVertex(vertex);
    color[vertex] = 0;
  }
  
  for (auto& edge : input.at("edges")) { g.AddEdge(edge.at("start"), edge.at("end")); }
  
  int result = MyAlgorithmFindCycle(g, &cycle, color);
  
  (*output)["result"] = result;
  if (result) {
    for (size_t vertex : cycle) {
      (*output)["cycle"].push_back(vertex);
    }
  }
  
  return 0;
}
}
