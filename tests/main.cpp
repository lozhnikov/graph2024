/**
 * @file tests/main.cpp
 * @author Mikhail Lozhnikov
 *
 * Файл с функией main() для клиентской части программы (набора тестов).
 */

#include <httplib.h>
#include "test.hpp"
#include "test_core.hpp"

int main(int argc, char* argv[]) {
  // Порт по-умолчанию.
  int port = 8080;
  // Адрес сервера по-умолчанию.
  std::string host = "127.0.0.1";

  // Обычные тесты на сборку для базовых шаблонов.
  // Сюда ничего добавлять не нужно.
  TestGraph();
  TestOrientedGraph();
  TestWeightedGraph();
  TestWeightedOrientedGraph();

  if (argc >= 2) {
    // Меняем хост, если предоставлен соответствующий аргумент командной строки.
    host = std::string(argv[1]);
  }

  if (argc >= 3) {
    // Меняем порт, если предоставлен соответствующий аргумент командной строки.
    if (std::sscanf(argv[2], "%d", &port) != 1)
      return -1;
  }

  // Адрес подключения в формате хост:порт.
  std::string url = host + ":" + std::to_string(port);

  httplib::Client cli(url.c_str());

  /* Сюда нужно вставить вызов набора тестов для алгоритма. */

<<<<<<< HEAD
  TestMyAlgorithmPrims(&cli);
=======
  const std::string body = u8" {\"0\":[1,2], \"1\":[0,2], \"2\":[0,1,3], \"3\":[2,4,5,6,7], \"4\":[3,5], \"5\":[3,4,9], \"6\":[3,7], \"7\":[3,6,8], \"8\":[7], \"9\":[5]} ";
  
  auto res_post = cli.Post("/find_bridges", body, "application/json");
  const std::string result = u8"{\" 2\":\"3\",\"3\":\"2\",\"5\":\"9\",\"7\":\"8\",\"8\":\"7\",\"9\":\"5\"}";

  REQUIRE_EQUAL(res_post->body, result);
  /*
  if (res_post->body == result ) std::cout << "Bce Ok" << std::endl;
    else std::cout << res_post->body << std::endl;
    */
>>>>>>> 7ede227 (Первая вариант задачи поиска мостов в графе)

  /* Конец вставки. */

  // Отправляем GET запрос для остановки сервера.
  httplib::Result res = cli.Get("/stop");

  if (res->status != 200)
    return -1;

  return TestSuite::Status();
}
