#include <iostream>
#include "matching_edmonds.hpp"

int main() {
    // Создание графа
    // Например, используя вашу реализацию graph::Graph

    // Вызов алгоритма нахождения наибольшего паросочетания
    std::vector<std::pair<int, int>> matchingPairs = MatchingEdmonds(graph);

    // Вывод результатов
    std::cout << "Размер наибольшего паросочетания: " << matchingPairs.size() << std::endl;
    std::cout << "Пары вершин в паросочетании:" << std::endl;
    for (const auto& pair : matchingPairs) {
        std::cout << pair.first << " - " << pair.second << std::endl;
    }

    return 0;
}
