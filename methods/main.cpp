/**
 * @file methods/main.cpp
 * @author Mikhail Lozhnikov
 *
 * Файл с функией main() для серверной части программы.
 */

#include <httplib.h>
#include <string>
#include <unordered_map>
#include <iostream>
#include <nlohmann/json.hpp>
#include "methods.hpp"

int main(int argc, char* argv[]) {
  // Порт по-умолчанию.
  int port = 8080;

  if (argc >= 2) {
    // Меняем порт по умолчанию, если предоставлен соответствующий
    // аргумент командной строки.
    if (std::sscanf(argv[1], "%d", &port) != 1)
      return -1;
  }

  std::cerr << "Listening on port " << port << "..." << std::endl;

  httplib::Server svr;

  // Обработчик для GET запроса по адресу /stop. Этот обработчик
  // останавливает сервер.
  svr.Get("/stop", [&](const httplib::Request&, httplib::Response& res) {
    std::cout << u8"Сервер выключается..." << std::endl;
    res.set_content(u8"Server выклбчился успешно!", "text/plain");
    svr.stop();
  });

  // Сюда нужно вставить обработчик post запроса для алгоритма.


  svr.Post("/Kruskal", [&](const httplib::Request& req,
                           httplib::Response& res) {
      nlohmann::json input = nlohmann::json::parse(req.body);
      nlohmann::json output;

      if (graph::KruskalMethod(input, &output) < 0)
          res.status = 400;

        res.set_content(output.dump(), "application/json");
  });

  svr.Post("/MyAlgorithmPrims", [&](const httplib::Request& req,
                                    httplib::Response& res) {
    nlohmann::json input = nlohmann::json::parse(req.body);
    nlohmann::json output;

    if (graph::MyAlgorithmPrimsMethod(input, &output) < 0)
      res.status = 400;

    res.set_content(output.dump(), "application/json");
  });

  svr.Post("/KuhnMatching", [&](const httplib::Request& req,
    httplib::Response& res) {

      nlohmann::json input = nlohmann::json::parse(req.body);
      nlohmann::json output;

      if (graph::KuhnMatchingMethod(input, &output) < 0)
        res.status = 400;

      res.set_content(output.dump(), "application/json");
    });

  svr.Post("/FordBellman", [&](const httplib::Request& req,
    httplib::Response& res) {
      nlohmann::json input = nlohmann::json::parse(req.body);
      nlohmann::json output;

      if (graph::FordBellmanMethod(input, &output) < 0)
        res.status = 400;

      res.set_content(output.dump(), "application/json");
    });

  /* Конец вставки. */

  // парсинг графа

  svr.Post("/find_bridges",
          [](const httplib::Request& req, httplib::Response& res) {
            graph::ProcessFindBrigdes(&req, &res);
          });
  // Конец вставки.

  // Эта функция запускает сервер на указанном порту. Программа не завершится
  // до тех пор, пока сервер не будет остановлен.
  svr.listen("0.0.0.0", port);

  return 0;
}
