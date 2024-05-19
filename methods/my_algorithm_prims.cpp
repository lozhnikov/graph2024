#include <nlohmann/json.hpp>
#include <weighted_graph.hpp>
#include <my_algorithm_prims.hpp>

namespace graph {

    template<typename WeightType>
    int MyAlgorithmPrimsHelperMethod(const nlohmann::json& input,
                                     nlohmann::json* output);

    int MyAlgorithmPrimsMethod(const nlohmann::json& input,
                               nlohmann::json* output) {
        std::string graphType = input.at("graph_type");

        if (graphType != "WeightedGraph") {
            return -1;
        }

        std::string weightType = input.at("weight_type");
        if (weightType == "int") {
            return MyAlgorithmPrimsHelperMethod<int>(input, output);
        } else if (weightType == "double") {
            return MyAlgorithmPrimsHelperMethod<double>(input, output);
        } else {
            return -1;
        }

        return -1;
    }

    template<typename WeightType>
    int MyAlgorithmPrimsHelperMethod(const nlohmann::json& input,
                                     nlohmann::json* output) {
        graph::WeightedGraph<WeightType> g;
        std::vector<std::pair<size_t, size_t>> result;

        for (auto& vertex : input.at("vertices")) {
            g.AddVertex(vertex);
        }

        for (auto& edge : input.at("edges")) {
            g.AddEdge(edge.at("from"), edge.at("to"), edge.at("weight"));
        }

        MyAlgorithmPrims<WeightType>(g, &result);

        for (size_t i = 0; i < result.size(); ++i) {
            (*output)["result"].push_back({result[i].first, result[i].second});
        }

        return 0;
    }
}  // namespace graph
