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
  tmp["vertices"] = std::vector<int>{ 11, 17, 52, 33, 42, 15, 46, 67 };

  tmp["edges"][0]["from"] = 11;
  tmp["edges"][0]["to"] = 15;

  tmp["edges"][1]["from"] = 11;
  tmp["edges"][1]["to"] = 42;

  tmp["edges"][2]["from"] = 52;
  tmp["edges"][2]["to"] = 46;

  tmp["edges"][3]["from"] = 33;
  tmp["edges"][3]["to"] = 67;

  std::string input = tmp.dump();

  auto res = cli->Post("/KuhnMatching", input, "application/json");

  if (!res) {
    REQUIRE(false);
  }

  nlohmann::json output = nlohmann::json::parse(res->body);

  std::vector<std::pair<size_t, size_t>> result = output.at("result");

  std::unordered_set<size_t> expected = { 11, 42, 52, 46, 33, 67 };
  std::unordered_set<size_t> resultEdges;

  for (int i = 0; i < result.size(); ++i) {
    resultEdges.insert(result[i].first);
    resultEdges.insert(result[i].second);
  }

  REQUIRE_EQUAL(expected, resultEdges);
}

static void RandomTest(httplib::Client* cli) {
  nlohmann::json tmp;
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> vert(0, 100);

  int Vert0 = vert(gen);
  int Vert1 = vert(gen);
  int Vert2 = vert(gen);
  int Vert3 = vert(gen);
  int Vert4 = vert(gen);
  int Vert5 = vert(gen);
  int Vert6 = vert(gen);
  int Vert7 = vert(gen);
  int Vert8 = vert(gen);
  int Vert9 = vert(gen);

  tmp["graph_type"] = "Graph";
  tmp["vertices"] = std::vector<int>{ Vert0, Vert1, Vert2, Vert3, Vert4,
  Vert5, Vert6, Vert7, Vert8, Vert9};

  tmp["edges"][0]["from"] = Vert0;
  tmp["edges"][0]["to"] = Vert5;

  tmp["edges"][1]["from"] = Vert0;
  tmp["edges"][1]["to"] = Vert9;

  tmp["edges"][2]["from"] = Vert1;
  tmp["edges"][2]["to"] = Vert7;

  tmp["edges"][3]["from"] = Vert2;
  tmp["edges"][3]["to"] = Vert8;

  tmp["edges"][4]["from"] = Vert0;
  tmp["edges"][4]["to"] = Vert6;

  tmp["edges"][5]["from"] = Vert3;
  tmp["edges"][5]["to"] = Vert8;

  tmp["edges"][6]["from"] = Vert1;
  tmp["edges"][6]["to"] = Vert9;

  tmp["edges"][7]["from"] = Vert4;
  tmp["edges"][7]["to"] = Vert7;

  tmp["edges"][8]["from"] = Vert4;
  tmp["edges"][8]["to"] = Vert6;

  tmp["edges"][9]["from"] = Vert3;
  tmp["edges"][9]["to"] = Vert5;



  std::string input = tmp.dump();

  auto res = cli->Post("/KuhnMatching", input, "application/json");

  if (!res) {
    REQUIRE(false);
  }

  nlohmann::json output = nlohmann::json::parse(res->body);

  std::vector<std::pair<size_t, size_t>> result = output.at("result");

  std::unordered_set<size_t> expected;
  std::unordered_set<size_t> resultEdges;

  for (int i = 0; i < result.size(); ++i) {
    resultEdges.insert(result[i].first);
    resultEdges.insert(result[i].second);
  }

  expected = resultEdges;

  REQUIRE_EQUAL(expected, resultEdges);
}
