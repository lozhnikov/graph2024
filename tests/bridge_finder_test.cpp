#include <httplib.h>
#include <nlohmann/json.hpp>
#include "test_core.hpp"



void GraphTestWithBridges1(httplib::Client* cli) {
  nlohmann::json request = nlohmann::json::parse(
    R"( {"0":[1,2], "1":[0,2], "2":[0,1,3], "3":[2,4,5,6,7], "4":[3,5], "5":[3,4,9], "6":[3,7], "7":[3,6,8], "8":[7], "9":[5]} )");
  auto resPost = cli->Post("/find_bridges", request.dump(), "application/json");
  REQUIRE(resPost == true);
  REQUIRE(resPost->status == 200);
  nlohmann::json result = nlohmann::json::parse(resPost->body);
  nlohmann::json expected = nlohmann::json::parse(
    R"({"2":"3","3":"2","5":"9","7":"8","8":"7","9":"5"})");
  REQUIRE_EQUAL(result, expected);
}

void GraphTestWithBridges2(httplib::Client* cli) {
  nlohmann::json request = nlohmann::json::parse(
    R"( {"0":[1], "1":[0,2], "2":[1,3,4], "3":[2,4], "4":[2,3,5], "5":[4,6,7], "6":[5,7], "7":[5,6]} )");
  auto resPost = cli->Post("/find_bridges", request.dump(), "application/json");
  REQUIRE(resPost == true);
  REQUIRE(resPost->status == 200);
  nlohmann::json result = nlohmann::json::parse(resPost->body);
  nlohmann::json expected = nlohmann::json::parse(
    R"({"0":"1","1":"0","2":"1","4":"5","5":"4"})");
  REQUIRE_EQUAL(result, expected);
}

void GraphTestWithNoBridges(httplib::Client* cli) {
  nlohmann::json request =
    nlohmann::json::parse(R"( {"0":[1,2], "1":[0,2], "2":[0,1]} )");
  auto resPost = cli->Post("/find_bridges", request.dump(), "application/json");
  REQUIRE(resPost == true);
  REQUIRE(resPost->status == 200);
  nlohmann::json result = nlohmann::json::parse(resPost->body);
  nlohmann::json expected = nlohmann::json::parse(R"({})");
  REQUIRE_EQUAL(result, expected);
}

std::unordered_map<size_t, size_t> graphCreator(size_t numVertices,
size_t minVertexNum) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> distrib(1, numVertices - 1);
  std::vector<size_t> beginGraph(numVertices);
  std::iota(beginGraph.begin(), beginGraph.end(), minVertexNum);
  std::vector<size_t> endGraph(beginGraph);
  std::rotate(endGraph.rbegin(), endGraph.rbegin() + distrib(gen),
    endGraph.rend());
  std::unordered_map<size_t, size_t> graph;
  for (size_t i = 0; i < beginGraph.size(); ++i) {
    graph[beginGraph[i]] = endGraph[i];
  }
  return graph;
}

void GraphRandomTest(httplib::Client* cli) {
  std::unordered_map<size_t, size_t> upperGraph;
  std::unordered_map<size_t, size_t> lowerGraph;
  upperGraph = graphCreator(100, 0);
  lowerGraph = graphCreator(100, 100);
  nlohmann::json request;
  for (auto& [from, to] : upperGraph) {
    request[std::to_string(from)] = {(to)};
  }
  for (auto& [from, to] : lowerGraph) {
    request[std::to_string(from)] = {(to)};
  }
  std::pair<size_t, size_t> bridge(upperGraph.begin()->second,
  lowerGraph.begin()->second);
  request[std::to_string(bridge.first)] = {bridge.second};
  auto resPost = cli->Post("/find_bridges", request.dump(), "application/json");
  REQUIRE(resPost == true);
  REQUIRE(resPost->status == 200);
  nlohmann::json result = nlohmann::json::parse(resPost->body);
  bool flag = false;
  for (auto& [key, value] : result.items()) {
    if (value == std::to_string(bridge.first)) {
      if (key == std::to_string(bridge.second)) {
        flag = true;
        break;
      }
    }
    if (key == std::to_string(bridge.first)) {
      if (value == std::to_string(bridge.second)) {
        flag = true;
        break;
      }
    }
  }
  REQUIRE(flag);
}

void TestFindBridges(httplib::Client* cli) {
  TestSuite suite("Test bridge finder in graph");

  RUN_TEST_REMOTE(suite, cli, GraphTestWithBridges1);
  RUN_TEST_REMOTE(suite, cli, GraphTestWithBridges2);
  RUN_TEST_REMOTE(suite, cli, GraphTestWithNoBridges);
  RUN_TEST_REMOTE(suite, cli, GraphRandomTest);
}
