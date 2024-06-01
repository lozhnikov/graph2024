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
  std::random_device rand;
  std::mt19937 g(rand());
  std::uniform_real_distribution<double> wght(2.0, 10.0);
  std::uniform_int_distribution<int> intgr(3, 15);

  std::set<int> v;
  tmp["graph_type"] = "WeightedOrientedGraph";
  tmp["weight_type"] = "double";
  double arr[15][15];
  for(int i = 0; i < 15; i++) {
    for(int j = 0; j < 15; j++) {
      arr[i][j] = -100;
    }
  }

  for (int i = 0; i < intgr(g); ++i) {
    tmp["edges"][i]["from"] = intgr(g);
    tmp["edges"][i]["to"] = intgr(g);
    tmp["edges"][i]["weight"] = wght(g);
    std::cout << "(" << tmp["edges"][i]["from"] << ", " << tmp["edges"][i]["to"] << ") = " << tmp["edges"][i]["weight"] << std::endl;
    v.insert(int(tmp["edges"][i]["from"]));
    v.insert(int(tmp["edges"][i]["to"]));
    arr[int(tmp["edges"][i]["from"])][int(tmp["edges"][i]["to"])] = double(tmp["edges"][i]["weight"]);
  }
  tmp["vertices"] = v;
/*  tmp["graph_type"] = "WeightedOrientedGraph";
  tmp["weight_type"] = "double";
  tmp["vertices"] = std::set<int> {0, 1, 2, 3, 4};

  tmp["edges"][0]["from"] = 0;
  tmp["edges"][0]["to"] = 1;
  tmp["edges"][0]["weight"] = 1.0;

  tmp["edges"][1]["from"] = 1;
  tmp["edges"][1]["to"] = 2;
  tmp["edges"][1]["weight"] = 2.0;

  double w = wght(g);
  tmp["edges"][2]["from"] = 1;
  tmp["edges"][2]["to"] = 3;
  tmp["edges"][2]["weight"] = w;

  tmp["edges"][3]["from"] = 4;
  tmp["edges"][3]["to"] = 3;
  tmp["edges"][3]["weight"] = 1.0;

  tmp["edges"][4]["from"] = 2;
  tmp["edges"][4]["to"] = 4;
  tmp["edges"][4]["weight"] = 2.0;*/

  std::string input = tmp.dump();

  auto res = cli->Post("/FordBellman", input, "application/json");

  if (!res) {
    REQUIRE(false);
  }

  nlohmann::json output = nlohmann::json::parse(res->body);

  std::map<size_t, double> result = output.at("result");
  
  for(int i = 0; i < 15; ++i) {
    for(int j = 0; j < 15; ++j) {
      std::cout << arr[i][j] << "  ";
    }
    std::cout << std::endl;
  }

  int from = 0;
  int to = 0;
  double sum = 0;
  for (int i = 0; i < 100; ++i) {
    from = 0;
    to = intgr(g);
    sum = 0;
    while(arr[from][to] > -1) {
      sum += arr[from][to];
      from = to;
      to = intgr(g);
    }
    if(sum < result[to]) {
      REQUIRE_EQUAL(1, -1);
    }
  }
/*  std::map<size_t, double> expected;
  expected[0] = 0.0;
  expected[1] = 1.0;
  expected[2] = 3.0;
  expected[3] = 6.0;
  expected[4] = 5.0;

  REQUIRE_EQUAL(expected, result);*/
}
