#include <httplib.h>
#include <nlohmann/json.hpp>
#include <string>
#include "bridge_finder.hpp"
#include "methods.hpp"

namespace graph {

void process_find_brigdes(const httplib::Request& req, httplib::Response& res) {

  std::string body_str = req.body;
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
  if (result.empty()) {
    res.set_content("{}", "application/json");
    return;
  };
  nlohmann::json j;
  for (auto& [from, to] : result) {
    j[std::to_string(from)] = std::to_string(to);
  }
  res.set_content(j.dump(), "application/json");
  
}

}  // namespace graph