#include <httplib.h>
#include <string>
#include <nlohmann/json.hpp>
#include "bridge_finder.hpp"
#include "methods.hpp"


namespace graph {

void ProcessFindBrigdes(const httplib::Request* req, httplib::Response* res) {
  std::string bodyStr = req->body;
  auto jsonBody =
      nlohmann::json::parse(bodyStr.begin(), bodyStr.end());
  graph::Graph gr;
  for (auto& el : jsonBody.items()) {
    size_t vertex = std::stoull(el.key());
    if (el.value().is_array()) {
      for (auto value : el.value()) {
        gr.AddEdge(vertex, value.get<std::size_t>());
      }
    }
  }

  auto result = graph::BridgeFinder(&gr);
  if (result.empty()) {
    res->set_content("{}", "application/json");
    return;
  }
  nlohmann::json j;
  for (auto& [from, to] : result) {
    j[std::to_string(from)] = std::to_string(to);
  }
  res->set_content(j.dump(), "application/json");
}
}  // namespace graph
