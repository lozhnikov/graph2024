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

<<<<<<< HEAD
int MyAlgorithmPrimsMethod(const nlohmann::json& input, nlohmann::json* output);
=======
  void process_find_brigdes(const httplib::Request& req, httplib::Response& res);

>>>>>>> 7ede227 (Первая вариант задачи поиска мостов в графе)

/* Конец вставки. */

}  // namespace graph

#endif  // METHODS_METHODS_HPP_
