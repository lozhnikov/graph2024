/**
 * @file ford_bellman_test.cpp
 * @author Yaroslav Egorov
 * 
 * Тесты для алгоритма graph::FordBellman.
 */

#include <httplib.h>
#include <vector>
#include <map>
#include <random>
#include <unordered_set>
#include <nlohmann/json.hpp>
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

  std::map<size_t, int> result = output.at("result");

  std::map<size_t, int> expected;

  expected[0] = 0;
  expected[1] = 1;
  expected[2] = 3;
  expected[3] = 6;
  expected[4] = 5;

  REQUIRE_EQUAL(expected, result);
}

static void RandomTest(httplib::Client* cli) {
  nlohmann::json tmp;
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<double> weight(7.0, 10.0);
  std::uniform_int_distribution<int> num(5, 14);
  std::uniform_int_distribution<int> integer(0, 14);

  std::set<int> v;
  tmp["graph_type"] = "WeightedOrientedGraph";
  tmp["weight_type"] = "double";
  double arr[15][15];
  for (int i = 0; i < 15; i++) {
    for (int j = 0; j < 15; j++) {
      arr[i][j] = 0.0;
    }
  }
  int edges = num(gen);
  for (int i = 0; i < edges; ++i) {
    tmp["edges"][i]["from"] = integer(gen);
    tmp["edges"][i]["to"] = integer(gen);
    tmp["edges"][i]["weight"] = weight(gen);

    v.insert(static_cast<int>(tmp["edges"][i]["from"]));
    v.insert(static_cast<int>(tmp["edges"][i]["to"]));
    arr[static_cast<int>(tmp["edges"][i]["from"])]
       [static_cast<int>(tmp["edges"][i]["to"])] =
       static_cast<double>(tmp["edges"][i]["weight"]);
  }
  tmp["vertices"] = v;

  std::string input = tmp.dump();

  auto res = cli->Post("/FordBellman", input, "application/json");

  if (!res) {
    REQUIRE(false);
  }

  nlohmann::json output = nlohmann::json::parse(res->body);

  std::map<size_t, double> result = output.at("result");

  int a = 0;
  int b = 0;
  int temp = 0;
  double sum = 0;
  int count = 0;
  for (int i = 0; i < 100; ++i) {
    a = 0;
    b = 0;
    temp = integer(gen);
    sum = 0;
    count = 0;
    while (arr[a][temp] > 1 && count < 10) {
      sum += arr[a][temp];
      b = temp;
      a = temp;
      temp = integer(gen);
      count++;
    }
    if (sum < result[b]) {
      REQUIRE_EQUAL(1, -1);
    }
  }
}
