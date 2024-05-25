#ifndef KRUSKAL_HPP_
#define KRUSKAL_HPP_

#include <vector>
#include <utility>
#include <algorithm>

namespace graph {

  using std::vector;
  using std::pair;

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

  vector<pair<int,int>> Kruskal(vector<pair<int,pair<int,int>>> g, int numVertices) {

    vector<pair<int,int>> result;

    sort(g.begin(), g.end());

    DisjointSet ds(numVertices);

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
