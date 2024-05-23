#include <httplib.h>
#include <nlohmann/json.hpp>
#include "test_core.hpp"



void GraphTestWithBridges1(httplib::Client* cli) {
  nlohmann::json request = nlohmann::json::parse(
      R"( {"0":[1,2], "1":[0,2], "2":[0,1,3], "3":[2,4,5,6,7], "4":[3,5], "5":[3,4,9], "6":[3,7], "7":[3,6,8], "8":[7], "9":[5]} )");
  auto resPost = cli->Post("/find_bridges", request.dump(), "application/json");
  REQUIRE(resPost == true);
  REQUIRE(resPost->status == 200);
  nlohmann::json result = nlohmann::json::parse(resPost->body);
  nlohmann::json expected = nlohmann::json::parse(
      R"({"2":"3","3":"2","5":"9","7":"8","8":"7","9":"5"})");
  REQUIRE_EQUAL(result, expected);
}

void GraphTestWithBridges2(httplib::Client* cli) {
  nlohmann::json request = nlohmann::json::parse(
      R"( {"0":[1], "1":[0,2], "2":[1,3,4], "3":[2,4], "4":[2,3,5], "5":[4,6,7], "6":[5,7], "7":[5,6]} )");
  auto resPost = cli->Post("/find_bridges", request.dump(), "application/json");
  REQUIRE(resPost == true);
  REQUIRE(resPost->status == 200);
  nlohmann::json result = nlohmann::json::parse(resPost->body);
  nlohmann::json expected = nlohmann::json::parse(
      R"({"0":"1","1":"0","2":"1","4":"5","5":"4"})");
  REQUIRE_EQUAL(result, expected);
}

void GraphTestWithNoBridges(httplib::Client* cli) {
  nlohmann::json request =
      nlohmann::json::parse(R"( {"0":[1,2], "1":[0,2], "2":[0,1]} )");
  auto resPost = cli->Post("/find_bridges", request.dump(), "application/json");
  REQUIRE(resPost == true);
  REQUIRE(resPost->status == 200);
  nlohmann::json result = nlohmann::json::parse(resPost->body);
  nlohmann::json expected = nlohmann::json::parse(R"({})");
  REQUIRE_EQUAL(result, expected);
}

void GraphTestWithBridges3(httplib::Client* cli) {
  nlohmann::json request =
      nlohmann::json::parse(R"( {"0":[1,2], "1":[0], "2":[0]} )");
  auto resPost = cli->Post("/find_bridges", request.dump(), "application/json");
  REQUIRE(resPost == true);
  REQUIRE(resPost->status == 200);
  nlohmann::json result = nlohmann::json::parse(resPost->body);
  nlohmann::json expected = nlohmann::json::parse(
      R"({"0":"1","1":"0","2":"0"})");
  REQUIRE_EQUAL(result, expected);
}

void TestFindBridges(httplib::Client* cli) {
  TestSuite suite("Test bridge finder in graph");

  RUN_TEST_REMOTE(suite, cli, GraphTestWithBridges1);
  RUN_TEST_REMOTE(suite, cli, GraphTestWithBridges2);
  RUN_TEST_REMOTE(suite, cli, GraphTestWithNoBridges);
  RUN_TEST_REMOTE(suite, cli, GraphTestWithBridges3);
}
