#ifndef INCLUDE_KRUSKAL_HPP_
#define INCLUDE_KRUSKAL_HPP_

#include <vector>
#include <utility>
#include <algorithm>

#include <set>
#include <unordered_map>
#include <weighted_graph.hpp>

namespace graph {

struct DisjointSet {
std::unordered_map<int, int> parent;
std::unordered_map<int, int> rank;

void makeSet(int x) {
    parent[x] = x;
    rank[x] = 0;
}

int find(int x) {
    if (parent.find(x) == parent.end()) {
        makeSet(x);
    }
    if (x != parent[x]) {
        parent[x] = find(parent[x]);
    }
    return parent[x];
}

void Union(int x, int y) {
    int xRoot = find(x);
    int yRoot = find(y);

    if (xRoot != yRoot) {
        if (rank[xRoot] < rank[yRoot]) {
            parent[xRoot] = yRoot;
        } else if (rank[xRoot] > rank[yRoot]) {
            parent[yRoot] = xRoot;
        } else {
            parent[yRoot] = xRoot;
            rank[xRoot]++;
        }
    }
}
};


template<typename Weight>
std::vector<std::pair<int, int>>
Kruskal(const graph::WeightedGraph<Weight>& graph) {
    std::vector<std::pair<int, int>> result;

    std::set<std::pair<Weight, std::pair<int, int>>> s;

    for (auto v1 : graph.Vertices()) {
      auto vs = graph.Edges(v1);
      for (auto v2 : vs) {
        std::pair<Weight, std::pair<int, int>> p;
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

    std::vector<std::pair<Weight, std::pair<int, int>>> g;
    for (auto e : s) {
      g.push_back(e);
    }

    sort(g.begin(), g.end());

    DisjointSet ds;

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

}  //  namespace graph
#endif  // INCLUDE_KRUSKAL_HPP_
