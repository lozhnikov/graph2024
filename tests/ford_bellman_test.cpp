/**
 * @file ford_bellman_test.cpp
 * @author Yaroslav Egorov
 * 
 * Тесты для алгоритма graph::FordBellman.
 */

#include <httplib.h>
#include <unordered_set>
#include <nlohmann/json.hpp>
#include <random>
#include <vector>
#include "test_core.hpp"

static void SimpleTest(httplib::Client* cli);
static void RandomTest(httplib::Client* cli);

void TestFordBellman(httplib::Client* cli) {
  TestSuite suite("TestFordBellman");
  TestSuite random_suite("RandomTestFordBellman");

  RUN_TEST_REMOTE(suite, cli, SimpleTest);
  RUN_TEST_REMOTE(random_suite, cli, RandomTest);
}

static void SimpleTest(httplib::Client* cli) {
  nlohmann::json tmp;
  tmp["graph_type"] = "WeightedOrientedGraph";
  tmp["weight_type"] = "int";
  tmp["vertices"] = std::vector<int>{0, 1, 2, 3, 4};
  
  tmp["edges"][0]["from"] = 0;
  tmp["edges"][0]["to"] = 1;
  tmp["edges"][0]["weight"] = 1;

  tmp["edges"][1]["from"] = 1;
  tmp["edges"][1]["to"] = 2;
  tmp["edges"][1]["weight"] = 2;

  tmp["edges"][2]["from"] = 1;
  tmp["edges"][2]["to"] = 3;
  tmp["edges"][2]["weight"] = 7;

  tmp["edges"][3]["from"] = 4;
  tmp["edges"][3]["to"] = 3;
  tmp["edges"][3]["weight"] = 1;

  tmp["edges"][4]["from"] = 2;
  tmp["edges"][4]["to"] = 4;
  tmp["edges"][4]["weight"] = 2;
  
  std::string input = tmp.dump();
  auto res = cli->Post("/FordBellman", input, "application/json");

  if (!res) {
    REQUIRE(false);
  }
  
  nlohmann::json output = nlohmann::json::parse(res->body);

  std::vector<int> result = output.at("result");

  std::unordered_set<int> expected = {0, 1, 3, 6, 5};
  std::unordered_set<int> resultSet;

  for (int id : result) {
    resultSet.insert(id);
  }

  REQUIRE_EQUAL(expected, resultSet);
}
static void RandomTest(httplib::Client* cli) {
  nlohmann::json tmp;
  std::random_device rd1;
  std::mt19937 gen1(rd1());
  std::uniform_real_distribution<double> weight1(9.0, 10.0);

  tmp["graph_type"] = "WeightedOrientedGraph";
  tmp["weight_type"] = "double";
  tmp["vertices"] = std::vector<int> {0, 1, 2, 3, 4};

  tmp["edges"][0]["from"] = 0;
  tmp["edges"][0]["to"] = 1;
  tmp["edges"][0]["weight"] = 1.0;

  tmp["edges"][1]["from"] = 1;
  tmp["edges"][1]["to"] = 2;
  tmp["edges"][1]["weight"] = 2.0;

  double w = weight1(gen1);   
  tmp["edges"][2]["from"] = 1;
  tmp["edges"][2]["to"] = 3;
  tmp["edges"][2]["weight"] = (double)w;

  tmp["edges"][3]["from"] = 4;
  tmp["edges"][3]["to"] = 3;
  tmp["edges"][3]["weight"] = 1.0;

  tmp["edges"][4]["from"] = 2;
  tmp["edges"][4]["to"] = 4;
  tmp["edges"][4]["weight"] = 2.0;

  std::string input = tmp.dump();

  auto res = cli->Post("/FordBellman", input, "application/json");

  if (!res) {
    REQUIRE(false);
  }

  nlohmann::json output = nlohmann::json::parse(res->body);

  std::vector<double> result = output.at("result");

  std::unordered_set<double> expected = {0, 1, 3, 6, 5};
  
  std::unordered_set<double> resultSet;
  
  for(int id : result) {
    resultSet.insert(id);
  }
  REQUIRE_EQUAL(expected, resultSet);
}
