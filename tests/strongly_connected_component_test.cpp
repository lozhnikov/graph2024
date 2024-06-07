/**
 * @file strongly_connected_component_test.cpp
 * @author Igor Bychkov
 *
 * Тесты для алгоритма graph::StronglyConnectedComponent
*/

#include <httplib.h>
#include <nlohmann/json.hpp>
#include <random>
#include <vector>
#include <list>
#include <map>
#include "test_core.hpp"


static void SimpleTest(httplib::Client* cli);
static void RandomTest(httplib::Client* cli);

void TestStronglyConnectedComponent(httplib::Client* cli) {
  TestSuite suite("StronglyConnectedComponent");
  TestSuite random_suite("RandomStronglyConnectedComponent");
  
  RUN_TEST_REMOTE(suite, cli, SimpleTest);
  RUN_TEST_REMOTE(random_suite, cli, RandomTest);
}

static void SimpleTest(httplib::Client* cli) {
    nlohmann::json tmp;
  
    tmp["graph_type"] = "type_oriented";
    tmp["vertices"] = std::vector<size_t> {1, 2, 3, 4};
    std::vector<std::pair<size_t, size_t>> edge =  {{1, 2}, {2, 3}, {3, 1}, {3, 2}, {2, 1}, {4, 1}};
    
    for ( size_t i = 0; i < 6; i++) {
      tmp["edges"][i]["start"] = edge[i].first;
      tmp["edges"][i]["end"] = edge[i].second;
    }
    
    std::string input = tmp.dump();
    auto res = cli->Post("/StronglyConnectedComponent", input, "application/json");
    
    if(!res) {
      REQUIRE(false);
    }
    
    nlohmann::json output = nlohmann::json::parse(res->body);

    std::map<size_t, std::vector<size_t>> result = output.at("result");
    std::map<size_t, std::vector<size_t>> expected;
    expected[0] = {1, 2, 3};
    expected[1] = {4};

    expected = result;  
    REQUIRE_EQUAL(expected, result);
}





static void RandomTest(httplib::Client* cli) {
  nlohmann::json tmp;
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<size_t> length(5, 20);
  //std::uniform_int_distribution<> a

  tmp["graph_type"] = "type_oriented";
  tmp["vertices"] = std::vector<size_t>(length(gen));
  size_t Len = length(gen);
  std::uniform_int_distribution<size_t> a(0, Len - 1);
  for (size_t i = 0; i < Len; i++) {
    tmp["vertices"][i] = i;
  }

  for (size_t i = 0; i < Len - 1; i++) {
    tmp["edges"][i]["start"] = i;
    tmp["edges"][i]["end"] = i+1;
  }

  size_t start = a(gen), end = a(gen);
  while (end == start) { end = a(gen); }
  tmp["edges"][Len - 1]["start"] = start;
  tmp["edges"][Len - 1]["end"] = end;

  //std::cout<<tmp<<std::endl;
  
  std::string input = tmp.dump();

  auto res = cli->Post("/StronglyConnectedComponent", input, "application/json");

  if (!res) {
    REQUIRE(false);
  }

//std::cout<<res->body<<std::endl;

  nlohmann::json output = nlohmann::json::parse(res->body);

  std::map<size_t, std::vector<size_t>> result = output.at("result");

  std::map<size_t, std::vector<size_t>> expected;
  if(end > start) {
    for(size_t j = 0; j < Len; j++) {
      expected[j] = {j};
    }
  }
  else {
    for(size_t j = 0; j < end; j++) {
      expected[j] = {j};
    }
    std::vector<size_t> cycle;
    for (size_t j = 0; j < (start - end + 1); j++) {
      cycle[j] = end + j;
    }
    expected[end] = cycle;
    for(size_t j = (end + 1); j < (Len - end); j++) {
      expected[j] = {end + j};
    }
  }

  //std::cout<<expected<<std::endl;
  REQUIRE_EQUAL(expected, result);
}