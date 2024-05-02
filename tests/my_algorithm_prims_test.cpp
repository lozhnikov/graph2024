/**
 * @file my_algorithm_prims_test.hpp
 * @author Bogdan Kushnarenko
 *
 * Тесты для алгоритма graph::MyAlgorithmPrims.
 */

#include <httplib.h>
#include <nlohmann/json.hpp>
#include <vector>
#include <unordered_set>
#include "test_core.hpp"

static void SimpleTest(httplib::Client* cli);

void TestMyAlgorithmPrims(httplib::Client* cli) {
    TestSuite suite("TestMyAlgorithmPrims");

    RUN_TEST_REMOTE(suite, cli, SimpleTest);
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

    std::vector<std::vector<int>> result = output.at("result");

    std::unordered_set<int> expected = {0, 1, 1, 2, 2, 4, 4, 3};
    std::unordered_set<int> resultEdges;

    for (int i = 0; i < result.size(); ++i) {
        resultEdges.insert(result[i][0]);
        resultEdges.insert(result[i][1]);
    }

    REQUIRE_EQUAL(expected, resultEdges);
}
