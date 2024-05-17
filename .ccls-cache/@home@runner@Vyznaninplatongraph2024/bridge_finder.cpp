#include <iostream>
#include <list>
#include <algorithm>

using namespace std;

class Graph {
    int V; // Количество вершин в графе
    list<int> *adj; // Список смежности Представляет собой массив списков, где каждый список содержит смежные вершины для соответствующей вершины графа. Используется для хранения связей между вершинами в графе.
    int time; // Время обхода

    void Dfs(int u, bool visited[], int disc[], int low[], int parent[]) {
        visited[u] = true; // Помечаем вершину как посещенную
        disc[u] = low[u] = ++time; // Инициализируем время обхода и минимальное время обхода

        for (auto v : adj[u]) {
            if (!visited[v]) {
                parent[v] = u; // Запоминаем родителя
                  Dfs(v, visited, disc, low, parent); // Рекурсивный вызов для смежной вершины

                low[u] = min(low[u], low[v]); // Обновляем минимальное время обхода

                if (low[v] > disc[u]) // Проверяем условие моста
                    cout << u << " " << v << endl; // Выводим мост
            } else if (v != parent[u]) {
                low[u] = min(low[u], disc[v]); // Обновляем минимальное время обхода
            }
        }
    }

public:
    Graph(int V) {
        this->V = V;
        adj = new list<int>[V];
        time = 0;
    }

    void addEdge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    void findBridges() {
        bool *visited = new bool[V];
        int *disc = new int[V];
        int *low = new int[V];
        int *parent = new int[V];

        for (int i = 0; i < V; i++) {
            parent[i] = -1;
            visited[i] = false;
        }

        for (int i = 0; i < V; i++) {
            if (!visited[i])
                  Dfs(i, visited, disc, low, parent);
        }
    }
};

int main() {
    Graph g(5); 
    g.addEdge(0, 1);
    g.addEdge(1, 2);
    g.addEdge(2, 0);
    g.addEdge(1, 3);
    g.addEdge(3, 4);

    cout << "Мосты в графе:\n";
    g.findBridges();

    return 0;
}