/**
 * @file my_algorithm_prims_test.hpp
 * @author Bogdan Kushnarenko
 *
 * Тесты для алгоритма graph::MyAlgorithmPrims.
 */

#include <httplib.h>
#include <vector>
#include <unordered_set>
#include <nlohmann/json.hpp>
#include <random>
#include "test_core.hpp"

static void SimpleTest(httplib::Client* cli);
static void RandomTest(httplib::Client* cli);

void TestMyAlgorithmPrims(httplib::Client* cli) {
    TestSuite suite("TestMyAlgorithmPrims");
    TestSuite random_suite("RandomTestMyAlgorithmPrims");

    RUN_TEST_REMOTE(suite, cli, SimpleTest);
    RUN_TEST_REMOTE(random_suite, cli, RandomTest);
}

static void SimpleTest(httplib::Client* cli) {
    nlohmann::json tmp;

    tmp["graph_type"] = "WeightedGraph";
    tmp["weight_type"] = "int";
    tmp["vertices"] = std::vector<int> {0, 1, 2, 3, 4};

    tmp["edges"][0]["from"] = 0;
    tmp["edges"][0]["to"] = 1;
    tmp["edges"][0]["weight"] = 5;

    tmp["edges"][1]["from"] = 1;
    tmp["edges"][1]["to"] = 2;
    tmp["edges"][1]["weight"] = 8;

    tmp["edges"][2]["from"] = 1;
    tmp["edges"][2]["to"] = 3;
    tmp["edges"][2]["weight"] = 11;

    tmp["edges"][3]["from"] = 4;
    tmp["edges"][3]["to"] = 3;
    tmp["edges"][3]["weight"] = 6;

    tmp["edges"][4]["from"] = 2;
    tmp["edges"][4]["to"] = 4;
    tmp["edges"][4]["weight"] = 9;

    std::string input = tmp.dump();

    auto res = cli->Post("/MyAlgorithmPrims", input, "application/json");

    if (!res) {
        REQUIRE(false);
    }

    nlohmann::json output = nlohmann::json::parse(res->body);

    std::vector<std::pair<size_t, size_t>> result = output.at("result");

    std::unordered_set<size_t> expected = {0, 1, 1, 2, 2, 4, 4, 3};
    std::unordered_set<size_t> resultEdges;

    for (int i = 0; i < result.size(); ++i) {
        resultEdges.insert(result[i].first);
        resultEdges.insert(result[i].second);
    }

    REQUIRE_EQUAL(expected, resultEdges);
}

static void RandomTest(httplib::Client* cli) {
    nlohmann::json tmp;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> weight(7.0, 10.0);

    tmp["graph_type"] = "WeightedGraph";
    tmp["weight_type"] = "double";
    tmp["vertices"] = std::vector<int> {11, 5, 10, 15, 13, 17, 19};

    tmp["edges"][0]["from"] = 11;
    tmp["edges"][0]["to"] = 5;
    tmp["edges"][0]["weight"] = 1.0;

    tmp["edges"][1]["from"] = 5;
    tmp["edges"][1]["to"] = 10;
    tmp["edges"][1]["weight"] = 2.0;

    tmp["edges"][2]["from"] = 10;
    tmp["edges"][2]["to"] = 15;
    tmp["edges"][2]["weight"] = 3.0;

    tmp["edges"][3]["from"] = 15;
    tmp["edges"][3]["to"] = 13;
    tmp["edges"][3]["weight"] = 4.0;

    tmp["edges"][4]["from"] = 13;
    tmp["edges"][4]["to"] = 17;
    tmp["edges"][4]["weight"] = 5.0;

    tmp["edges"][5]["from"] = 17;
    tmp["edges"][5]["to"] = 19;
    tmp["edges"][5]["weight"] = 6.0;

    double firstWeight = weight(gen);
    tmp["edges"][6]["from"] = 11;
    tmp["edges"][6]["to"] = 19;
    tmp["edges"][6]["weight"] = firstWeight;

    double secondWeight = weight(gen);
    tmp["edges"][7]["from"] = 10;
    tmp["edges"][7]["to"] = 17;
    tmp["edges"][7]["weight"] = secondWeight;

    std::string input = tmp.dump();

    auto res = cli->Post("/MyAlgorithmPrims", input, "application/json");

    if (!res) {
        REQUIRE(false);
    }

    nlohmann::json output = nlohmann::json::parse(res->body);

    std::vector<std::pair<size_t, size_t>> result = output.at("result");

    std::unordered_set<size_t> expected = {11, 5, 5, 10, 10, 15, 13, 13, 17, 17, 19};
    std::unordered_set<size_t> resultEdges;

    for (int i = 0; i < result.size(); ++i) {
        resultEdges.insert(result[i].first);
        resultEdges.insert(result[i].second);
    }

    REQUIRE_EQUAL(expected, resultEdges);
}
