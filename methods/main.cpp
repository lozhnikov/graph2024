/**
 * @file methods/main.cpp
 * @author Mikhail Lozhnikov
 *
 * Файл с функией main() для серверной части программы.
 */

#include <httplib.h>
#include <iostream>
#include <nlohmann/json.hpp>
#include <string>
#include <unordered_map>
//#include "bridge_finder.hpp"
#include "methods.hpp"

int main(int argc, char* argv[]) {
/*
  graph::Graph Gr;
  Gr.AddEdge(0, 1);
  Gr.AddEdge(1, 2);
  Gr.AddEdge(0, 2);
  Gr.AddEdge(2, 3);

  Gr.AddEdge(3, 4);
  Gr.AddEdge(3, 6);
  Gr.AddEdge(3, 7);
  Gr.AddEdge(6, 7);
  Gr.AddEdge(7, 8);
  Gr.AddEdge(4, 5);
  Gr.AddEdge(3, 5);
  Gr.AddEdge(5, 9);

  auto result = graph::bridge_finder(Gr);
*/
  // Порт по-умолчанию.
  int port = 8080;

  if (argc >= 2) {
    // Меняем порт по умолчанию, если предоставлен соответствующий
    // аргумент командной строки.
    if (std::sscanf(argv[1], "%d", &port) != 1)
      return -1;
  }

  std::cerr << "Listening on port " << port << "..." << std::endl;

  httplib::Server srv;

  // Обработчик для GET запроса по адресу /stop. Этот обработчик
  // останавливает сервер.
  srv.Get("/stop", [&](const httplib::Request&, httplib::Response& res) {
    std::cout << u8"Сервер выключается..." << std::endl;
    res.set_content(u8"Server выклбчился успешно!", "text/plain");
    srv.stop();
  });

  //Сюда нужно вставить обработчик post запроса для алгоритма.
  srv.Get("/hi", [](const httplib::Request&, httplib::Response& res) {
    std::cout << u8"Запрос по url /hi" << std::endl;
    res.set_content("Hello World!", "text/plain");
  });

  //парсинг графа

  srv.Post("/find_bridges",
          [](const httplib::Request& req, httplib::Response& res) {
            graph::process_find_brigdes(req, res);
          });
  
  // Конец вставки.

  // Эта функция запускает сервер на указанном порту. Программа не завершится
  // до тех пор, пока сервер не будет остановлен.
  srv.listen("0.0.0.0", port);

  return 0;
}
