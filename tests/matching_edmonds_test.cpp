#include <matching_edmonds.hpp>
#include <httplib.h>
#include <algorithm>
#include <random>
#include <iostream>
#include <utility>
#include <vector>
#include <graph.hpp>
#include <nlohmann/json.hpp>
#include "test_core.hpp"

using graph::Graph;

void TestMatchingEdmondsCore(httplib::Client *cli) {
  /**
   * @brief Тесты алгоритма matching_edmonds.
   *
   * @param cli Клиент, отправляющий запросы.
   *
   * Функция тестирует алгоритм, Отправляя JSON-ы с клиента, и
   * проверяя выходные JSON-ы на корректность.
  */
  std::map<std::string, std::pair<nlohmann::json, nlohmann::json>> cases;
  std::vector<std::pair<size_t, size_t>> empty_list;
  cases["Empty"] = {
    {
      {"vertices", empty_list},
      {"edges", empty_list}
    },
    {
      {"greatest matching", 0}
    }
  };
  cases["One line"] = {
    {
      {"vertices", {1, 2, 3, 10}},
      {"edges", {{1, 2}, {2, 3}, {3, 10}}}
    },
    {
      {"greatest matching", 2}
    }
  };
  cases["No edges"] = {
    {
      {"vertices", {1, 2, 3, 4}},
      {"edges", empty_list}
    },
    {
      {"greatest matching", 2}
    }
  };
  cases["All connected"] = {
    {
      {"vertices", {1, 2, 3}},
      {"edges", {{1, 2}, {2, 3}, {3, 1}}}
    },
    {
      {"greatest matching", 1}
    }
  };
  cases["Multiple Connectivity components"] = {
    {
      {"vertices", {1, 2, 3, 4, 5}},
      {"edges", {{1, 2}, {3, 4}}}
    },
    {
      {"greatest matching", 3}
    }
  };
  for (const auto &[name, value] : cases) {
    std::cout << name << "... ";
    auto output = cli->Post("/MatchingEdmonds",
                            value.first.dump(), "application/json");
    REQUIRE(output->body == value.second.dump());
    REQUIRE_EQUAL(output->body, value.second.dump());
    std::cout << "OK!\n";
  }

  std::cout << "Random test... ";
  int vertices_num = 100;
  int edges_num = 150;

  std::vector<size_t> vertices(vertices_num);
  std::iota(vertices.begin(), vertices.end(), 0);
  std::bernoulli_distribution bern(0.5);
  std::random_device rd;
  std::mt19937 generator(rd());
  std::uniform_int_distribution dist(1, vertices_num-1);

  for (int j = 0; j < 100; ++j) {
    std::vector<std::pair<size_t, size_t>> edges;
    int threshold = dist(generator);
    std::uniform_int_distribution first_part(0, threshold-1);
    std::uniform_int_distribution second_part(threshold, vertices_num-1);
    for (int i = 0; i < edges_num; ++i) {
      size_t vert1;
      size_t vert2;
      if (bern(generator)) {
        vert1 = first_part(generator);
        vert2 = first_part(generator);
      } else {
        vert1 = second_part(generator);
        vert2 = second_part(generator);
      }
      if (vert1 > vert2) {
        std::swap(vert1, vert2);
      }
      edges.push_back({vert1, vert2});
    }
    edges.push_back({threshold-1, threshold});
    sort(edges.begin(), edges.end());
    edges.erase(unique(edges.begin(), edges.end()), edges.end());
    nlohmann::json random_graph = {
        {"vertices", vertices},
        {"edges", edges}
    };
    auto output = cli->Post("/MatchingEdmonds",
                            random_graph.dump(), "application/json");
    std::stringstream ss;
    ss << threshold-1 << "," << threshold;
    std::string bridge = ss.str();
    REQUIRE(output->body.find(bridge) != std::string::npos);
  }
  std::cout << "OK!\n";
}

void TestMatchingEdmonds(httplib::Client *cli) {
    TestSuite suite("TestMatchingEdmonds");
    RUN_TEST_REMOTE(suite, cli, TestMatchingEdmondsCore);
}
