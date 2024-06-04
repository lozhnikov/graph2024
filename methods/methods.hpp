/**
 * @file methods/methods.hpp
 * @author Mikhail Lozhnikov
 *
 * Объявления функий для серверной части алгоритмов. Эти функции должны
 * считать JSON, который прислал клиент, выполнить алгоритм и отправить клиенту
 * JSON с результатом работы алгоритма.
 */

#ifndef METHODS_METHODS_HPP_
#define METHODS_METHODS_HPP_

namespace graph {

/* Сюда нужно вставить объявление серверной части алгоритма. */

int MyAlgorithmPrimsMethod(const nlohmann::json& input, nlohmann::json* output);

void ProcessFindBrigdes(const httplib::Request* req, httplib::Response* res);

int KruskalMethod(const nlohmann::json& input, nlohmann::json* output);

int MyAlgorithmPrimsMethod(const nlohmann::json& input, nlohmann::json* output);
int FordBellmanMethod(const nlohmann::json& input, nlohmann::json* output);

int KuhnMatchingMethod(const nlohmann::json& input, nlohmann::json* output);

/* Конец вставки. */

}  // namespace graph

#endif  // METHODS_METHODS_HPP_
