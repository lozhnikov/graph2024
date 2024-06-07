#include <nlohmann/json.hpp>
#include <oriented_graph.hpp>
#include <strongly_connected_component.hpp>
#include <map>
#include<list>

namespace graph {
int StronglyConnectedComponentMethod(const nlohmann::json& input, nlohmann::json* output) {
  std::string GraphType = input.at("graph_type");
  if(GraphType != "type_oriented"){ return -1; }
  
  
  graph::OrientedGraph graph;
  std::map<size_t, std::vector<size_t>> result;
  
  for (auto& vertex : input.at("vertices")) { 
    graph.AddVertex(vertex);
  }
  
  for (auto& edge : input.at("edges")) { graph.AddEdge(edge.at("start"), edge.at("end")); }
  
  StronglyConnectedComponent(&graph,&result);
  for (size_t i = 0; i < result.size(); ++i) {
      (*output)["result"].push_back(result[i]);
  }  
  return 0;
}
}