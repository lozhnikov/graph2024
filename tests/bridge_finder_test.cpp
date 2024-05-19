#include <httplib.h>
#include <nlohmann/json.hpp>
#include "test_core.hpp"



void graph_test_with_bridges1(httplib::Client* cli) {
  nlohmann::json request = nlohmann::json::parse(
      R"( {"0":[1,2], "1":[0,2], "2":[0,1,3], "3":[2,4,5,6,7], "4":[3,5], "5":[3,4,9], "6":[3,7], "7":[3,6,8], "8":[7], "9":[5]} )");
  auto res_post = cli->Post("/find_bridges", request.dump(), "application/json");
  REQUIRE(res_post == true);
  REQUIRE(res_post->status == 200);
  nlohmann::json result = nlohmann::json::parse(res_post->body);
  nlohmann::json expected = nlohmann::json::parse(
      R"({"2":"3","3":"2","5":"9","7":"8","8":"7","9":"5"})");
  REQUIRE_EQUAL(result, expected);
}

void graph_test_with_bridges2(httplib::Client* cli) {
  nlohmann::json request = nlohmann::json::parse(
      R"( {"0":[1], "1":[0,2], "2":[1,3,4], "3":[2,4], "4":[2,3,5], "5":[4,6,7], "6":[5,7], "7":[5,6]} )");
  auto res_post = cli->Post("/find_bridges", request.dump(), "application/json");
  REQUIRE(res_post == true);
  REQUIRE(res_post->status == 200);
  nlohmann::json result = nlohmann::json::parse(res_post->body);
  nlohmann::json expected = nlohmann::json::parse(
      R"({"0":"1","1":"0","2":"1","4":"5","5":"4"})");
  REQUIRE_EQUAL(result, expected);
}

void graph_test_with_no_bridges(httplib::Client* cli) {
  nlohmann::json request =
      nlohmann::json::parse(R"( {"0":[1,2], "1":[0,2], "2":[0,1]} )");
  auto res_post = cli->Post("/find_bridges", request.dump(), "application/json");
  REQUIRE(res_post == true);
  REQUIRE(res_post->status == 200);
  nlohmann::json result = nlohmann::json::parse(res_post->body);
  nlohmann::json expected = nlohmann::json::parse(R"({})");
  REQUIRE_EQUAL(result, expected);
}

void graph_test_with_bridges3(httplib::Client* cli) {
  nlohmann::json request =
      nlohmann::json::parse(R"( {"0":[1,2], "1":[0], "2":[0]} )");
  auto res_post = cli->Post("/find_bridges", request.dump(), "application/json");
  REQUIRE(res_post == true);
  REQUIRE(res_post->status == 200);
  nlohmann::json result = nlohmann::json::parse(res_post->body);
  nlohmann::json expected = nlohmann::json::parse(
      R"({"0":"1","1":"0","2":"0"})");
  REQUIRE_EQUAL(result, expected);
}

void test_find_bridges(httplib::Client& cli) {

  TestSuite suite1("Test bridge finder with bridges big graph");
  TestSuite suite2("Test bridge finder with bridges small graph");
  TestSuite suite3("Test bridge finder with no bridges");
  TestSuite suite4("Test bridge finder with bridges extra small graph");

  RUN_TEST_REMOTE(suite1, &cli, graph_test_with_bridges1);
  RUN_TEST_REMOTE(suite2, &cli, graph_test_with_bridges2);
  RUN_TEST_REMOTE(suite3, &cli, graph_test_with_no_bridges);
  RUN_TEST_REMOTE(suite4, &cli, graph_test_with_bridges3);
}
