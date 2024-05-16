#include <httplib.h>
#include <nlohmann/json.hpp>
#include <string>
#include "bridge_finder.hpp"
#include "methods.hpp"

namespace graph {

void process_find_brigdes(const httplib::Request& req, httplib::Response& res) {
/*
  using namespace nlohmann;
  using namespace graph;

  const auto parseGraph = [](const std::string& body) -> const graph::Graph {
    const auto reqBody = json::parse(body.cbegin(), body.cend());
    graph::Graph g;
    for (auto& item : reqBody.items()) {
      const auto key = item.key();
      const auto keyVal = std::stoull(key);
      if (item.value().is_array()) {
        for (auto value : item.value()) {
          g.AddEdge(keyVal, value.get<std::size_t>());
        }
      }
    }
    return g;
  };

   Graph g = parseGraph(req.body);
  const auto result = graph::bridge_finder(g);
  json j;
  for (const auto& [from, to] : result) {
    j[std::to_string(from)] = std::to_string(to);
  }
  res.set_content(j.dump(), "application/json");
  */
  
  std::string body_str = req.body;
  //std::cout << body_str << std::endl;
  auto json_body =
      nlohmann::json::parse(body_str.begin(), body_str.end());
  graph::Graph Gr;
  for (auto& el : json_body.items()) {
    size_t vertex = std::stoull(el.key());
    
    if (el.value().is_array()) {
      for (auto value : el.value()) {
        Gr.AddEdge(vertex, value.get<std::size_t>());
      }
    }
  }

  auto result = graph::bridge_finder(Gr);
  nlohmann::json j;
  for (auto& [from, to] : result) {
    j[std::to_string(from)] = std::to_string(to);
  }
  res.set_content(j.dump(), "application/json");
  
}

}  // namespace graph