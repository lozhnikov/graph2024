/**
 * @file breadth_first_traversal_test.cpp
 * @author Mikhail Lozhnikov
 *
 * Тесты для алгоритма graph::BreadthFirstTraversal.
 */

#include <httplib.h>
#include <unordered_set>
#include <nlohmann/json.hpp>
#include "test_core.hpp"

static void SimpleTest(httplib::Client* cli);

void TestBreadthFirstTraversal(httplib::Client* cli) {
  TestSuite suite("TestBreadthFirstTraversal");

  RUN_TEST_REMOTE(suite, cli, SimpleTest);
}

static void SimpleTest(httplib::Client* cli) {
  nlohmann::json tmp;

  tmp["graph_type"] = "Graph";
  tmp["vertices"] = std::vector<int>{1, 3, 5, 7, 9, 11};
  tmp["edges"][0]["from"] = 1;
  tmp["edges"][0]["to"] = 3;

  tmp["edges"][1]["from"] = 3;
  tmp["edges"][1]["to"] = 7;

  tmp["edges"][2]["from"] = 1;
  tmp["edges"][2]["to"] = 9;

  tmp["edges"][3]["from"] = 5;
  tmp["edges"][3]["to"] = 11;

  std::string input = tmp.dump();

/*
  std::string input = R"(
{
  "graph_type": "Graph",
  "vertices": [1, 3, 5, 7, 9, 11],
  "edges": [
    { "from": 1, "to": 3},
    { "from": 3, "to": 7},
    { "from": 1, "to": 9},
    { "from": 5, "to": 11}
  ]
}
)";
*/
  auto res = cli->Post("/BreadthFirstTraversal", input, "application/json");

  if (!res) {
    REQUIRE(false);
  }

  nlohmann::json output = nlohmann::json::parse(res->body);

  std::vector<int> result = output.at("result");

  std::unordered_set<int> expected = {1, 3, 5, 7, 9, 11};
  std::unordered_set<int> resultSet;

  for (int id : result)
    resultSet.insert(id);

  REQUIRE_EQUAL(expected, resultSet);
}

