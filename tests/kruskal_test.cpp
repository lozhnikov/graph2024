#include <kruskal.hpp>
#include <httplib.h>
#include <vector>
#include <utility>
#include <algorithm>
#include <random>
#include <unordered_set>
#include <nlohmann/json.hpp>
#include <iostream>
#include "test_core.hpp"

using std::vector;
using std::pair;

static void SimpleTest(httplib::Client* cli);
static void RandomTest(httplib::Client* cli);

void TestKruskal(httplib::Client* cli) {
  TestSuite suite("TestKruskal");

  RUN_TEST_REMOTE(suite, cli, SimpleTest);
  RUN_TEST_REMOTE(suite, cli, RandomTest);
}

bool ComparePairs(const pair<int,int>& p1, const pair<int,int>& p2) {
  return (p1.first == p2.first && p1.second == p2.second) ||
  (p1.first == p2.second && p1.second == p2.first);
}

bool CompareGraphs(const vector<pair<int,int>>& graph1,
                   const vector<pair<int,int>>& graph2) {

  if (graph1.size() != graph2.size()) {
    return false;
  }

  vector<pair<int,int>> sortedGraph1 = graph1;
  vector<pair<int,int>> sortedGraph2 = graph2;
  sort(sortedGraph1.begin(), sortedGraph1.end());
  sort(sortedGraph2.begin(), sortedGraph2.end());

  for (size_t i = 0; i < sortedGraph1.size(); ++i) {
    if (!ComparePairs(sortedGraph1[i], sortedGraph2[i])) {
      return false;
    }
  }

  return true;
}

static void SimpleTest(httplib::Client* cli) {

  nlohmann::json tmp;

  tmp["graph_type"] = "WeightedGraph";
  tmp["weight_type"] = "int";
  tmp["vertices"] = std::vector<int> {1, 2, 3, 4, 5, 6, 7};

  tmp["edges"][0]["from"] = 1;
  tmp["edges"][0]["to"] = 2;
  tmp["edges"][0]["weight"] = 1;

  tmp["edges"][1]["from"] = 1;
  tmp["edges"][1]["to"] = 3;
  tmp["edges"][1]["weight"] = 1;

  tmp["edges"][2]["from"] = 2;
  tmp["edges"][2]["to"] = 4;
  tmp["edges"][2]["weight"] = 2;

  tmp["edges"][3]["from"] = 2;
  tmp["edges"][3]["to"] = 5;
  tmp["edges"][3]["weight"] = 3;

  tmp["edges"][4]["from"] = 2;
  tmp["edges"][4]["to"] = 3;
  tmp["edges"][4]["weight"] = 4;

  tmp["edges"][5]["from"] = 5;
  tmp["edges"][5]["to"] = 6;
  tmp["edges"][5]["weight"] = 1;

  tmp["edges"][6]["from"] = 3;
  tmp["edges"][6]["to"] = 6;
  tmp["edges"][6]["weight"] = 5;

  tmp["edges"][7]["from"] = 3;
  tmp["edges"][7]["to"] = 7;
  tmp["edges"][7]["weight"] = 2;

  std::string input = tmp.dump();

  auto res = cli->Post("/Kruskal", input, "application/json");

  if (!res) {
    REQUIRE(false);
  }

  nlohmann::json output = nlohmann::json::parse(res->body);

  std::vector<std::pair<int,int>> result = output.at("edges");

  std::vector<std::pair<int,int>> expected =
  {{1, 2}, {1, 3}, {2, 4},
   {2, 5}, {5, 6}, {3, 7}};

  REQUIRE_EQUAL(true, CompareGraphs(result, expected));

}

static void RandomTest(httplib::Client* cli)
{
  int numOfTests = 10;
  const vector<pair<int,int>> expected = {
    {1, 2},
    {1, 3},
    {2, 5},
    {4, 6},
    {1, 4},
    {4, 7}
  };

  const vector<pair<int,int>> edges = {
    {1, 5},
    {1, 6},
    {1, 7},
    {2, 3},
    {2, 4},
    {2, 6},
    {2, 7},
    {3, 4},
    {3, 5},
    {3, 6},
    {3, 7},
    {4, 5},
    {5, 6},
    {5, 7},
    {6, 7}
  };

  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<int> coinFlip(0, 1);
  std::uniform_int_distribution<int> randomWeight(10, 30);

  for (int i = 0; i < numOfTests; i++) {

    vector<pair<int,pair<int,int>>> g = {
      {1, {1, 2}},
      {2, {1, 3}},
      {3, {2, 5}},
      {4, {4, 6}},
      {5, {1, 4}},
      {6, {4, 7}}
    };

    for (pair<int,int> e : edges) {
      if (coinFlip(gen) == 1) {
        int w = randomWeight(gen);
        pair<int,pair<int,int>> p;
        p.first = w;
        p.second = e;
        g.push_back(p);
      }
    }

    auto rng = std::default_random_engine {};
    std::shuffle(std::begin(g), std::end(g), rng);

    nlohmann::json tmp;

    tmp["graph_type"] = "WeightedGraph";
    tmp["weight_type"] = "int";
    tmp["vertices"] = std::vector<int> {1, 2, 3, 4, 5, 6, 7};

    int j = 0;
    for (pair<int,pair<int,int>> e : g) {
      tmp["edges"][j]["from"] = e.second.first;
      tmp["edges"][j]["to"] = e.second.second;
      tmp["edges"][j]["weight"] = e.first;
      j++;
    }

    std::string input = tmp.dump();

    auto res = cli->Post("/Kruskal", input, "application/json");

    if (!res) {
      REQUIRE(false);
    }

    nlohmann::json output = nlohmann::json::parse(res->body);

    std::vector<std::pair<int,int>> result = output.at("edges");

    REQUIRE_EQUAL(true, CompareGraphs(result, expected));
  }
}
