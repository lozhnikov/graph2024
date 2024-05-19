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
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> vert1(0, 3);
  std::uniform_int_distribution<> vert2(4, 7);


  tmp["graph_type"] = "Graph";
  tmp["vertices"] = std::vector<int>{ 0, 1, 2, 3, 4, 5, 6, 7 };

  tmp["edges"][0]["from"] = 0;
  tmp["edges"][0]["to"] = 4;

  tmp["edges"][1]["from"] = 1;
  tmp["edges"][1]["to"] = 5;

  tmp["edges"][2]["from"] = 2;
  tmp["edges"][2]["to"] = 6;

  tmp["edges"][3]["from"] = 3;
  tmp["edges"][3]["to"] = 7;

  int firstVert = vert1(gen);
  int secondVert = vert2(gen);
  tmp["edges"][4]["from"] = firstVert;
  tmp["edges"][4]["to"] = secondVert;

  firstVert = vert1(gen);
  secondVert = vert2(gen);
  tmp["edges"][5]["from"] = firstVert;
  tmp["edges"][5]["to"] = secondVert;

  std::string input = tmp.dump();

  auto res = cli->Post("/KuhnMatching", input, "application/json");

  if (!res) {
    REQUIRE(false);
  }

  nlohmann::json output = nlohmann::json::parse(res->body);

  std::vector<std::pair<size_t, size_t>> result = output.at("result");

  std::unordered_set<size_t> expected = { 3, 7, 2, 6, 1, 5, 0, 4 };
  std::unordered_set<size_t> resultEdges;

  for (int i = 0; i < result.size(); ++i) {
    resultEdges.insert(result[i].first);
    resultEdges.insert(result[i].second);
  }

  REQUIRE_EQUAL(expected, resultEdges);
}

static void RandomTest(httplib::Client* cli) {
  nlohmann::json tmp;
  unsigned int seed;
  srand(time(0));

  tmp["graph_type"] = "Graph";
  tmp["vertices"] = std::vector<int>{0, 1, 2, 3, 4, 5, 6, 7};

  tmp["edges"][0]["from"] = 0;
  tmp["edges"][0]["to"] = 4;

  tmp["edges"][1]["from"] = 1;
  tmp["edges"][1]["to"] = 5;

  tmp["edges"][2]["from"] = 2;
  tmp["edges"][2]["to"] = 6;

  tmp["edges"][3]["from"] = 3;
  tmp["edges"][3]["to"] = 7;

  int firstVert = rand_r(&seed) % 4;
  int secondVert = rand_r(&seed) % (4) + 4;
  tmp["edges"][4]["from"] = firstVert;
  tmp["edges"][4]["to"] = secondVert;

  firstVert = rand_r(&seed) % 4;
  secondVert = rand_r(&seed) % (4) + 4;
  tmp["edges"][5]["from"] = firstVert;
  tmp["edges"][5]["to"] = secondVert;

  std::string input = tmp.dump();

  auto res = cli->Post("/KuhnMatching", input, "application/json");

  if (!res) {
    REQUIRE(false);
  }

  nlohmann::json output = nlohmann::json::parse(res->body);

  std::vector<std::pair<size_t, size_t>> result = output.at("result");

  std::unordered_set<size_t> expected = {3, 7, 2, 6, 1, 5, 0, 4};
  std::unordered_set<size_t> resultEdges;

  for (int i = 0; i < result.size(); ++i) {
    resultEdges.insert(result[i].first);
    resultEdges.insert(result[i].second);
  }

  REQUIRE_EQUAL(expected, resultEdges);
}
