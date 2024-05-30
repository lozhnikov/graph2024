#ifndef KRUSKAL_HPP_
#define KRUSKAL_HPP_

#include <vector>
#include <utility>
#include <algorithm>

#include <set>
#include <weighted_graph.hpp>

namespace graph {

  struct DisjointSet {
    int *parent, *rank;
    int n;

    DisjointSet(int n) {
      this->n = n;
      parent = new int[n+1];
      rank = new int[n+1];

      for (int i = 0; i <= n; i++) {
        rank[i] = 0;
        parent[i] = i;
      }
    }

    int find(int x) {
      if (x != parent[x])
        parent[x] = find(parent[x]);
      return parent[x];
    }

    void Union(int x, int y) {
      int xRoot = find(x);
      int yRoot = find(y);

      if (rank[xRoot] < rank[yRoot])
        parent[xRoot] = yRoot;
      else if (rank[xRoot] > rank[yRoot])
        parent[yRoot] = xRoot;
      else {
        parent[yRoot] = xRoot;
        rank[xRoot]++;
      }
    }
  };

  template<typename Weight>
  std::vector<std::pair<int,int>> Kruskal(const graph::WeightedGraph<Weight>& graph) {

    std::vector<std::pair<int,int>> result;

    std::set<std::pair<Weight,std::pair<int,int>>> s;

    for (auto v1 : graph.Vertices()) {
      auto vs = graph.Edges(v1);
      for (auto v2 : vs) {
        std::pair<Weight,std::pair<int,int>> p;
        if (v1 < v2) {
          p.second.first = v1;
          p.second.second = v2;
        } else {
          p.second.first = v2;
          p.second.second = v1;
        }
        p.first = graph.EdgeWeight(v1, v2);
        s.insert(p);
      }
    }

    std::vector<std::pair<Weight,std::pair<int,int>>> g;
    for (auto e : s) {
      g.push_back(e);
    }

    sort(g.begin(), g.end());

    DisjointSet ds(graph.NumVertices());

    for (auto edge : g) {
      int src = edge.second.first;
      int dest = edge.second.second;
      int srcSet = ds.find(src);
      int destSet = ds.find(dest);
      if (srcSet != destSet) {
        result.push_back({src, dest});
        ds.Union(srcSet, destSet);
      }
    }

    return result;

  }

}

#endif
