/**
 * @file my_algorithm_findcycle_test.hpp
 * @author Oleg Malashevich
 *
 * Тесты для алгоритма graph::MyAlgorithmFindCycle
*/

#include <httplib.h>
#include <nlohmann/json.hpp>
#include <random>
#include <vector>
#include "test_core.hpp"

static void SimpleTest(httplib::Client* cli);
static void RandomTest(httplib::Client* cli);

void TestMyAlgorithmFindCycle(httplib::Client* cli) {
  TestSuite suite("TestMyAlgorithmFindCycle");
  TestSuite random_suite("RandomTestMyAlgorithmFindCycle");
  
  RUN_TEST_REMOTE(suite, cli, SimpleTest);
  RUN_TEST_REMOTE(random_suite, cli, RandomTest);
}

static void SimpleTest(httplib::Client* cli) {
  nlohmann::json tmp;
  
    tmp["graph_type"] = "type_oriented";
    tmp["vertices"] = std::vector<size_t> {0, 1, 2, 3, 4};
    std::vector<std::pair<size_t, size_t>> edge =  {{0, 1}, {1, 2}, {1, 3}, {2, 4}, {3, 2}, {4, 3}};
    
    for ( size_t i = 0; i < 6; i++) {
      tmp["edges"][i]["start"] = edge[i].first;
      tmp["edges"][i]["end"] = edge[i].second;
    }
    
    std::string input = tmp.dump();
    auto res = cli->Post("/MyAlgorithmFindCycle", input, "application/json");
    
    if(!res) {
      REQUIRE(false);
    }
    
    nlohmann::json output = nlohmann::json::parse(res->body);

    std::pair<int, std::vector<size_t>> result = {output.at("result"), output.at("cycle")};
    std::pair<int, std::vector<size_t>> expected = {1, {4, 3, 2, 4}};
    
    REQUIRE_EQUAL(expected, result);
}

static void RandomTest(httplib::Client* cli) {
  nlohmann::json tmp;
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<size_t> am_components(1, 5);
  
  size_t components = am_components(gen);
  
  std::uniform_int_distribution<size_t> vertice(0, 4 * components - 1);
  
  tmp["graph_type"] = "type_oriented";
  tmp["vertices"] = std::vector<size_t>(4 * components);
  std::vector<std::pair<size_t, size_t>> edge;
  
  for (size_t i = 0; i < (components * 4); i++) {
    tmp["vertices"][i] = i;
  }
  std::cout<<"tmp:"<<tmp<<std::endl;//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  for (size_t i = 0; i < components; i++) {
    edge.push_back({4 * i, 4 * i + 1});
    edge.push_back({4 * i + 1, 4 * i + 2});
    edge.push_back({4 * i + 3, 4 * i + 1});
  }
  size_t start = vertice(gen);
  size_t end = vertice(gen);
  
  while ((start == end) || (start == (end + 1)) || (start == (end - 1))){ end = vertice(gen); }
  edge.push_back({start, end});
  
  size_t i = 0;
  for (auto& edges : edge) {
    tmp["edges"][i]["start"] = edge[i].first;
    tmp["edges"][i]["end"] = edge[i].second;
    i++;
  }
  
  std::cout<<"edges: ";
  for (auto& edges : edge) {
    std::cout<<edges;
  }
  std::cout<<std::endl;
  
  std::string input = tmp.dump();
  std::cout<<"input: "<<input<<std::endl;
  
  auto res = cli->Post("/MyAlgorithmFindCycle", input, "application/json");
  
  std::cout<<res->body<<std::endl;
    
  if(!res) {
    REQUIRE(false);
  }
  
  nlohmann::json output = nlohmann::json::parse(res->body); 
  
  std::pair<int, std::vector<size_t>> result;
  
  if (output.at("result") == 1) {
    result = {output.at("result"), output.at("cycle")};
  }
  else { result = {output.at("result"), {}}; }
  
  std::pair<int, std::vector<size_t>> expected;
  
  if (((start / 4) == (end / 4)) && ((start % 4) == 2)) {
    if ((end % 4) == 0){
      expected = {1, {end + 1, start, end, end + 1}};
    }
    else if ((end % 4) == 3) {
      expected = {1, {start - 1, start, end, start - 1}};
    }
  }
  else { expected = {0, {}}; }
  
  std::cout<<"expected: "<<expected<<std::endl;
  
  REQUIRE_EQUAL(expected, result);
}
