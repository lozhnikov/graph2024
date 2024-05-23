//@file include/kuhn_matching.hpp
//@author Samarokov Aleksei
//Алгоритм Куна нахождения наибольшего паросочетания в двудольном графе.
 


#ifndef INCLUDE_KUHN_MATCHING_HPP_
#define INCLUDE_KUHN_MATCHING_HPP_
#include <iostream>
#include <vector>
#include <unordered_map>
#include <set>
#include <utility>
#include <unordered_set>

namespace graph {

struct Params {
    std::unordered_map < size_t, int> mt;
    std::unordered_map < size_t, int> used;
    std::unordered_map<size_t, std::unordered_set<size_t>> g;
};

bool HasCommonElement(const std::unordered_set<size_t>& set1,
  const std::unordered_set<size_t>& set2) {
  for (int element : set1) {
    if (set2.count(element) > 0) {
      return true;
    }
  }
  return false;
}

bool TryKuhn(size_t v, Params* params) {
  std::unordered_map < size_t, int>& mt = params->mt;
  std::unordered_map < size_t, int>& used = params->used;
  std::unordered_map<size_t, std::unordered_set<size_t>>& g = params->g;
  if (used[v] == 1)  return false;
  used[v] = 1;
  for (const auto& value : g[v]) {
    size_t to = value;
    if (mt[to] == -1 || TryKuhn(mt[to], params)) {
      mt[to] = v;
      return true;
    }
  }
  return false;
}

template<typename GraphType>
void KuhnMatching(GraphType* graph,
std::vector<std::pair<size_t, size_t>>* resEdges) {
  std::unordered_set<size_t> minVert = {};
  std::vector<std::pair<size_t, size_t>> edgesHelp;
  std::unordered_map<size_t, std::unordered_set<size_t>> firstPart;
  std::unordered_map<size_t, std::unordered_set<size_t>> secondPart;
  std::vector<size_t> helper;
  std::unordered_map < size_t, int> used1;
  Params params;

  for (size_t id : (*graph).Vertices()) {
    if ((*graph).Edges(id).empty() == true
      && (*graph).IncomingEdges(id).empty() == true) {
      helper.push_back(id);
    }
  }

  for (size_t id : helper) {
    (*graph).RemoveVertex(id);
  }

  for (size_t id : (*graph).Vertices()) {
    used1.insert({ id, -1 });
  }

  for (size_t id : (*graph).Vertices()) {
    if (HasCommonElement(minVert, (*graph).Edges(id)) == false) {
      firstPart.insert(std::pair<size_t,
        std::unordered_set<size_t>>(id, (*graph).Edges(id)));
      minVert.insert(id);
    } else {
      secondPart.insert(std::pair<size_t,
        std::unordered_set<size_t>>(id, (*graph).Edges(id)));
    }
  }

  for (const auto& [i, value] : firstPart) {
    params.g.insert(std::pair<size_t,
    std::unordered_set<size_t>>(i, value));
  }

  for (size_t id : (*graph).Vertices()) {
    params.mt.insert({id, -1});
  }

  for (const auto& [i, value] : firstPart)
    for (const auto& value : firstPart[i])
      if (params.mt[value] == -1) {
        params.mt[value] = i;
        used1[i] = 1;
        break;
      }

  for (const auto& [i, value] : firstPart) {
    if (used1[i] == 1)  continue;
    for (size_t id : (*graph).Vertices()) {
      params.used.insert({ id, -1 });
    }
    TryKuhn(i, &params);
  }

  for (const auto& [i, value] : secondPart)
    if (params.mt[i] != -1) {
      edgesHelp.push_back({ params.mt[i], i });
    }

  *resEdges = edgesHelp;
  }

}  // namespace graph
#endif  // INCLUDE_KUHN_MATCHING_HPP_
