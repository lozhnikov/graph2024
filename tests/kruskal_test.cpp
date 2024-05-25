#include <kruskal.hpp>
#include <httplib.h>
#include <vector>
#include <utility>
#include <algorithm>
#include <unordered_set>
#include <nlohmann/json.hpp>
#include "test_core.hpp"

using std::vector;
using std::pair;

static void SimpleTest(httplib::Client* cli);

void TestKruskal(httplib::Client* cli) {
  TestSuite suite("TestKruskal");

  RUN_TEST_REMOTE(suite, cli, SimpleTest);
}

bool ComparePairs(const pair<int, int>& p1, const pair<int, int>& p2) {
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

  std::string input = R"(
    {
      "graph": [
        [1, [1, 2]],
        [1, [1, 3]],
        [2, [2, 4]],
        [3, [2, 5]],
        [4, [2, 3]],
        [1, [5, 6]],
        [5, [3, 6]],
        [2, [3, 7]]
      ],
      "numVertices": 7
    }
  )";

  auto res = cli->Post("/Kruskal", input, "application/json");

  if (!res) {
    REQUIRE(false);
  }

  nlohmann::json output = nlohmann::json::parse(res->body);

  std::vector<std::pair<int,int>> result = output.at("edges");

  vector<pair<int,int>> expected =
  {{1, 2}, {1, 3}, {2, 4},
   {2, 5}, {5, 6}, {3, 7}};

  REQUIRE_EQUAL(true, CompareGraphs(result, expected));

}
