/**
 * @file matching_edmonds.hpp
 * @author Ivan Onishin
 *
 * Реализация алгоритма нахождения наибольшего паросочетания
 * в произвольных графах.
 */

#ifndef INCLUDE_MATCHING_EDMONDS_HPP_
#define INCLUDE_MATCHING_EDMONDS_HPP_

#include <algorithm>
#include <unordered_map>
#include <vector>
#include <cstring>
#include "graph.hpp"

const int MAXN = 1000;

template<class Graph>
int lca(const Graph &graph, int n, std::vector<std::vector<int>>& g, int* match,
  int* p, int* base, int* q, bool* used, bool* blossom, int a, int b) {
  std::vector<bool> visited(MAXN, false);
  // поднимаемся от вершины a до корня, помечая все чётные вершины
  for (;;) {
    a = base[a];
    visited[a] = true;
    if (match[a] == -1)  break;  // дошли до корня
    a = p[match[a]];
  }
  // поднимаемся от вершины b, пока не найдём помеченную вершину
  for (;;) {
    b = base[b];
    if (visited[b])  return b;
    b = p[match[b]];
  }
}

template<class Graph>
void mark_path(const Graph &graph, int n, std::vector<std::vector<int>>& g,
  int* match, int* p, int* base, int* q, bool* used,
  bool* blossom, int v, int b, int children) {
  while (base[v] != b) {
    blossom[base[v]] = blossom[base[match[v]]] = true;
    p[v] = children;
    children = match[v];
    v = p[match[v]];
  }
}

template<class Graph>
int find_path(const Graph &graph, int n, std::vector<std::vector<int>>& g,
  int* match, int* p, int* base, int* q, bool* used, bool* blossom,
  int root) {
  memset(used, 0, sizeof used);
  memset(p, -1, sizeof p);
  for (int i = 0; i < n; ++i)
    base[i] = i;

  used[root] = true;
  int qh = 0, qt = 0;
  q[qt++] = root;
  while (qh < qt) {
    int v = q[qh++];
    for (size_t i = 0; i < g[v].size(); ++i) {
      int to = g[v][i];
      if (base[v] == base[to] || match[v] == to)  continue;
      if (to == root || (match[to] != -1 && p[match[to]] != -1)) {
        int curbase = lca(graph, n, g, match, p, base, q, used, blossom, v, to);
        memset(blossom, 0, sizeof blossom);
        mark_path(graph, n, g, match, p, base, q, used, blossom, v, curbase,
          to);
        mark_path(graph, n, g, match, p, base, q, used, blossom, to, curbase,
          v);
        for (int i = 0; i < n; ++i)
          if (blossom[base[i]]) {
            base[i] = curbase;
            if (!used[i]) {
              used[i] = true;
              q[qt++] = i;
            }
          }
      } else if (p[to] == -1) {
        p[to] = v;
        if (match[to] == -1) return to;
        to = match[to];
        used[to] = true;
        q[qt++] = to;
      }
    }
  }
  return -1;
}

template<class Graph>
int MatchingEdmonds(const Graph &graph) {
  /**
   * @brief Алгоритм нахождения наибольшего паросочетания
   * в произвольных графах.
   *
   * @param graph Граф.
   *
   * На вход подаётся ссылка на объект типа graph::Graph, описанный в файле:
   * @sa graph.cpp
   * Алгоритм возвращает мощность наибольшего паросочетания.
   */

  int count = 0;
  int n = graph.NumVertices();
  std::vector<std::vector<int>> g(n);
  int* match = new int[n];
  int* p = new int[n];
  int* base = new int[n];
  int* q = new int[n];
  bool* used = new bool[n];
  bool* blossom = new bool[n];

  memset(match, -1, sizeof match);
  for (int i = 0; i < n; ++i)
    if (match[i] == -1) {
      int v = find_path(graph, n, g, match, p, base, q, used, blossom, i);
      while (v != -1) {
        int pv = p[v], ppv = match[pv];
        match[v] = pv;
        match[pv] = v;
        v = ppv;
      }
    }

  for (int i = 0; i < n; ++i) {
    if (match[i] != -1) {
      count++;
    }
  }

  delete[] match;
  delete[] p;
  delete[] base;
  delete[] q;
  delete[] used;
  delete[] blossom;

  return count / 2;
}

#endif  // INCLUDE_MATCHING_EDMONDS_HPP_
