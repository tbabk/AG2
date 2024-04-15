#ifndef __PROGTEST__
#include <cassert>
#include <cctype>
#include <cmath>
#include <cinttypes>
#include <limits>

#include <memory>
#include <string>
#include <random>
#include <algorithm>

#include <iostream>
#include <iomanip>

#include <optional>
#include <vector>
#include <deque>
#include <list>
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <queue>
#include <stack>

enum class Destination : uint8_t {
  UNKNOWN = 0, KTI, KAM
};

struct Plan {
  std::vector<Destination> cats;
  std::vector<std::vector<unsigned>> relationship;
};

using Result = std::pair<unsigned, std::vector<Destination>>;

#endif


Result split_cats(const Plan& plan) {
  // TODO
}


#ifndef __PROGTEST__


using enum Destination;
const std::vector<std::pair<unsigned, Plan>> test_data = {
  { 92, Plan{ { {}, {}, KAM, {}, KTI, {}, }, {
    { 0, 8, 1, 9, 9, 6, },
    { 8, 0, 14, 11, 14, 11, },
    { 1, 14, 0, 9, 7, 3, },
    { 9, 11, 9, 0, 12, 7, },
    { 9, 14, 7, 12, 0, 5, },
    { 6, 11, 3, 7, 5, 0, },
  } } },
  { 122, Plan{ { KAM, KAM, KTI, {}, KAM, KAM, {}, }, {
    { 0, 3, 3, 5, 9, 4, 7, },
    { 3, 0, 5, 15, 9, 14, 14, },
    { 3, 5, 0, 11, 12, 2, 6, },
    { 5, 15, 11, 0, 6, 3, 13, },
    { 9, 9, 12, 6, 0, 7, 12, },
    { 4, 14, 2, 3, 7, 0, 1, },
    { 7, 14, 6, 13, 12, 1, 0, },
  } } },
  { 133, Plan{ { KTI, {}, {}, KAM, KAM, {}, {}, KTI, }, {
    { 0, 13, 7, 10, 9, 3, 15, 4, },
    { 13, 0, 5, 5, 11, 6, 5, 12, },
    { 7, 5, 0, 15, 5, 8, 15, 15, },
    { 10, 5, 15, 0, 9, 15, 6, 9, },
    { 9, 11, 5, 9, 0, 8, 5, 8, },
    { 3, 6, 8, 15, 8, 0, 1, 5, },
    { 15, 5, 15, 6, 5, 1, 0, 10, },
    { 4, 12, 15, 9, 8, 5, 10, 0, },
  } } },
  { 219, Plan{ { {}, {}, KAM, KTI, {}, {}, {}, {}, {}, }, {
    { 0, 7, 2, 7, 5, 9, 8, 10, 4, },
    { 7, 0, 3, 15, 4, 4, 14, 1, 8, },
    { 2, 3, 0, 6, 13, 4, 6, 2, 10, },
    { 7, 15, 6, 0, 11, 10, 3, 4, 6, },
    { 5, 4, 13, 11, 0, 1, 14, 14, 15, },
    { 9, 4, 4, 10, 1, 0, 11, 11, 5, },
    { 8, 14, 6, 3, 14, 11, 0, 5, 3, },
    { 10, 1, 2, 4, 14, 11, 5, 0, 10, },
    { 4, 8, 10, 6, 15, 5, 3, 10, 0, },
  } } },
  { 311, Plan{ { KTI, {}, {}, {}, KAM, {}, {}, {}, {}, {}, }, {
    { 0, 4, 4, 10, 8, 8, 13, 2, 9, 2, },
    { 4, 0, 3, 5, 9, 3, 14, 15, 13, 12, },
    { 4, 3, 0, 7, 4, 14, 13, 9, 6, 5, },
    { 10, 5, 7, 0, 1, 10, 4, 10, 13, 12, },
    { 8, 9, 4, 1, 0, 12, 10, 4, 13, 2, },
    { 8, 3, 14, 10, 12, 0, 13, 8, 7, 13, },
    { 13, 14, 13, 4, 10, 13, 0, 2, 3, 15, },
    { 2, 15, 9, 10, 4, 8, 2, 0, 11, 3, },
    { 9, 13, 6, 13, 13, 7, 3, 11, 0, 13, },
    { 2, 12, 5, 12, 2, 13, 15, 3, 13, 0, },
  } } },
  { 327, Plan{ { {}, {}, {}, {}, KAM, {}, {}, {}, {}, KTI, KAM, }, {
    { 0, 3, 2, 8, 5, 15, 5, 1, 14, 8, 8, },
    { 3, 0, 8, 12, 9, 2, 8, 12, 11, 6, 6, },
    { 2, 8, 0, 3, 3, 1, 3, 6, 5, 6, 11, },
    { 8, 12, 3, 0, 8, 1, 8, 4, 10, 3, 2, },
    { 5, 9, 3, 8, 0, 8, 8, 15, 3, 13, 14, },
    { 15, 2, 1, 1, 8, 0, 8, 8, 6, 6, 10, },
    { 5, 8, 3, 8, 8, 8, 0, 10, 12, 15, 2, },
    { 1, 12, 6, 4, 15, 8, 10, 0, 14, 1, 9, },
    { 14, 11, 5, 10, 3, 6, 12, 14, 0, 9, 6, },
    { 8, 6, 6, 3, 13, 6, 15, 1, 9, 0, 9, },
    { 8, 6, 11, 2, 14, 10, 2, 9, 6, 9, 0, },
  } } },
  { 447, Plan{ { {}, {}, {}, {}, {}, {}, {}, {}, KTI, {}, KAM, {}, }, {
    { 0, 1, 12, 9, 9, 4, 11, 14, 8, 4, 9, 7, },
    { 1, 0, 5, 4, 9, 9, 5, 8, 9, 14, 12, 12, },
    { 12, 5, 0, 8, 12, 13, 14, 2, 1, 2, 4, 13, },
    { 9, 4, 8, 0, 6, 5, 1, 6, 5, 6, 15, 8, },
    { 9, 9, 12, 6, 0, 14, 11, 3, 9, 2, 13, 11, },
    { 4, 9, 13, 5, 14, 0, 11, 6, 6, 9, 8, 1, },
    { 11, 5, 14, 1, 11, 11, 0, 7, 13, 13, 9, 13, },
    { 14, 8, 2, 6, 3, 6, 7, 0, 4, 5, 3, 14, },
    { 8, 9, 1, 5, 9, 6, 13, 4, 0, 2, 10, 2, },
    { 4, 14, 2, 6, 2, 9, 13, 5, 2, 0, 1, 14, },
    { 9, 12, 4, 15, 13, 8, 9, 3, 10, 1, 0, 6, },
    { 7, 12, 13, 8, 11, 1, 13, 14, 2, 14, 6, 0, },
  } } },
};

void verify_result(unsigned h, const Plan& plan, const Result& res) {
  using enum Destination;
  auto&& [ happiness, dest ] = res;

  assert(dest.size() == plan.cats.size());

  for (size_t i = 0; i < dest.size(); i++) {
    assert(dest[i] == KAM || dest[i] == KTI);
    if (plan.cats[i] != UNKNOWN) assert(dest[i] == plan.cats[i]);
  }

  unsigned s = 0;
  for (size_t i = 0; i < dest.size(); i++)
    for (size_t j = 0; j < i; j++)
      if (dest[i] == dest[j]) s += plan.relationship[i][j];

  assert(s == happiness);
  assert(h == happiness);
}

int main() {
  for (const auto& [ h, plan ] : test_data)
    verify_result(h, plan, split_cats(plan));

  return 0;
}

#endif


