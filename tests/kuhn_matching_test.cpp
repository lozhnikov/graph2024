/**
 * @file my_algorithm_prims_test.hpp
 * @author Aleksei Samarokov
 *
 * Тесты для алгоритма graph:: KuhnMatching.
 */

#include <httplib.h>
#include <vector>
#include <random>
#include <nlohmann/json.hpp>
#include "test_core.hpp"

static void SimpleTest(httplib::Client* cli);
static void RandomTest(httplib::Client* cli);

void TestKuhnMatching(httplib::Client* cli) {
  TestSuite suite("TestKuhnMatching");
  TestSuite random_suite("RandomTestKuhnMatching");

  RUN_TEST_REMOTE(suite, cli, SimpleTest);
  RUN_TEST_REMOTE(random_suite, cli, RandomTest);
}

static void SimpleTest(httplib::Client* cli) {
  nlohmann::json tmp;

  tmp["graph_type"] = "Graph";
  tmp["vertices"] = std::vector<int>{ 1, 2, 3, 4, 5, 6, 7 };
  tmp["edges"][0]["from"] = 1;
  tmp["edges"][0]["to"] = 4;

  tmp["edges"][1]["from"] = 1;
  tmp["edges"][1]["to"] = 5;

  tmp["edges"][2]["from"] = 2;
  tmp["edges"][2]["to"] = 6;

  tmp["edges"][3]["from"] = 3;
  tmp["edges"][3]["to"] = 7;

  tmp["edges"][4]["from"] = 3;
  tmp["edges"][4]["to"] = 5;


  std::string input = tmp.dump();

  auto res = cli->Post("/KuhnMatching", input, "application/json");

  if (!res) {
      REQUIRE(false);
  }

  nlohmann::json output = nlohmann::json::parse(res->body);

  std::vector<std::pair<size_t, size_t>> result = output.at("result");

  std::unordered_set<size_t> expected = {3,5,2,6,1,4 };
  std::unordered_set<size_t> resultEdges;

  for (int i = 0; i < result.size(); ++i) {
    resultEdges.insert(result[i].first);
    resultEdges.insert(result[i].second);
  }

  REQUIRE_EQUAL(expected, resultEdges);
}

static void RandomTest(httplib::Client* cli) {
  nlohmann::json tmp;

  tmp["graph_type"] = "Graph";
  tmp["vertices"] = std::vector<int>{ 1,2,3,4,5,6,7,8 };

  tmp["edges"][0]["from"] = 1;
  tmp["edges"][0]["to"] = 5;

  tmp["edges"][1]["from"] = 2;
  tmp["edges"][1]["to"] = 6;

  tmp["edges"][2]["from"] = 3;
  tmp["edges"][2]["to"] = 7;

  tmp["edges"][3]["from"] = 4;
  tmp["edges"][3]["to"] = 8;

  int firstVert = rand() % 5;
  int secondVert = rand() % (4) + 5;
  tmp["edges"][4]["from"] = firstVert;
  tmp["edges"][4]["to"] = secondVert;

  firstVert = rand() % 5;
  secondVert = rand() % (4) + 5;
  tmp["edges"][5]["from"] = firstVert;
  tmp["edges"][5]["to"] = secondVert;

  firstVert = rand() % 5;
  secondVert = rand() % (4) + 5;
  tmp["edges"][6]["from"] = firstVert;
  tmp["edges"][6]["to"] = secondVert;

  firstVert = rand() % 5;
  secondVert = rand() % (4) + 5;
  tmp["edges"][7]["from"] = firstVert;
  tmp["edges"][7]["to"] = secondVert;


  std::string input = tmp.dump();

  auto res = cli->Post("/KuhnMatching", input, "application/json");

  if (!res) {
    REQUIRE(false);
  }

  nlohmann::json output = nlohmann::json::parse(res->body);

  std::vector<std::pair<size_t, size_t>> result = output.at("result");

  std::unordered_set<size_t> expected = { 4,8,3,7,2,6,1,5 };
  std::unordered_set<size_t> resultEdges;

  for (int i = 0; i < result.size(); ++i) {
    resultEdges.insert(result[i].first);
    resultEdges.insert(result[i].second);
  }

  REQUIRE_EQUAL(expected, resultEdges);
}
