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

#include <vector>
#include <deque>
#include <list>
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <queue>
#include <stack>

// rooms are numbered consecutively starting with zero
using Room = unsigned;
using Corridor = std::pair<Room, Room>;

inline constexpr Room NO_ROOM = std::numeric_limits<Room>::max();

struct Map {
  Room s;
  std::vector<Room> beds;
  std::vector<unsigned> food;
  std::vector<Corridor> corridors;
};

struct Result {
  Room final_bed;
  unsigned food_eaten;
};

bool operator == (Result a, Result b) {
  return a.final_bed == b.final_bed && (
    a.food_eaten == b.food_eaten || a.final_bed == NO_ROOM);
}

std::ostream& operator << (std::ostream& out, Result r) {
  out << "(room: ";
  if (r.final_bed == NO_ROOM) out << "NO_ROOM";
  else out << r.final_bed;
  return out << ", food: " << r.food_eaten << ")";
}

#endif


Result find_lost_cat(const Map& map) {
  // TODO
}


#ifndef __PROGTEST__


const std::vector<std::pair<Result, Map>> test_data = {
  // Simple
  { { 1, 4 }, Map{ 0, { 1 },
    { 0, 1, 1, 2, 1 },
    {
      { 0, 2 }, { 0, 3 },
      { 2, 4 }, { 3, 4 },
      { 0, 1 }, { 4, 1 }
    }
  }},
  { { 7, 8 }, Map{ 3, { 7 },
    {
       1,  1,  0,  0,  1,  2,  0,  0,  0,  1,  2,  2,
       2,
    }, {
      { 0,  2}, { 1,  8}, { 2,  4}, { 2,  7}, { 2, 11}, { 3,  6},
      { 3,  9}, { 3, 10}, { 4,  7}, { 5,  8}, { 6,  1}, { 6,  5},
      { 6,  8}, { 8,  0}, { 8,  2}, { 8, 12}, { 9,  6}, {10,  6},
      {11,  7}, {12,  2},
    }
  }},
  { { NO_ROOM, 0 }, Map{ 4, { 5 },
    {
       2,  1,  0,  0,  2,  0,  2,  2,  1,  0,  1,  0,
       1,
    }, {
      { 0,  2}, { 1,  5}, { 3,  0}, { 3,  2}, { 3, 10}, { 4,  3},
      { 5,  6}, { 5,  8}, { 5, 11}, { 6, 11}, { 7,  5}, { 8, 11},
      { 9,  1}, { 9,  5}, { 9,  7}, {10,  2}, {11,  3}, {11,  4},
      {11, 12}, {12,  3},
    }
  }},

  // Acyclic
  { { NO_ROOM, 0 }, Map{ 2, { 4 },
    {
       6,  1,  3,  0,  3,  6,  4,  0,
    }, {
      { 0,  1}, { 0,  6}, { 1,  6}, { 3,  0}, { 3,  2}, { 3,  6},
      { 4,  1}, { 4,  5}, { 5,  0}, { 5,  1}, { 5,  6}, { 6,  2},
      { 7,  2}, { 7,  3}, { 7,  4},
    }
  }},
  { { NO_ROOM, 0 }, Map{ 3, { 7 },
    {
       0,  0,  0,  2,  0,  6,  6,  0,
    }, {
      { 1,  0}, { 1,  3}, { 1,  5}, { 2,  1}, { 3,  5}, { 4,  2},
      { 5,  6}, { 7,  3}, { 7,  4}, { 7,  5}, { 7,  6},
    }
  }},
  { { 6, 6 }, Map{ 0, { 6 },
    {
       2,  2,  2,  1,  0,  0,  4,  6,
    }, {
      { 0,  5}, { 0,  6}, { 2,  1}, { 3,  2}, { 4,  1}, { 4,  2},
      { 5,  7}, { 6,  1}, { 6,  2}, { 6,  4}, { 6,  5}, { 6,  7},
      { 7,  1},
    }
  }},

  // General
  { { 8, 23 }, Map{ 1, { 8 },
    {
       3,  6,  4,  0,  6,  4,  6,  3,  0,  3,
    }, {
      { 0,  8}, { 1,  5}, { 2,  6}, { 2,  9}, { 3,  0}, { 3,  2},
      { 3,  8}, { 4,  2}, { 4,  8}, { 5,  3}, { 5,  8}, { 6,  0},
      { 7,  2}, { 7,  3}, { 8,  1}, { 8,  9},
    }
  }},
  { { NO_ROOM, 0 }, Map{ 6, { 7 },
    {
       5,  0,  0,  1,  5,  0,  0,  0,  2,  5,
    }, {
      { 0,  4}, { 0,  9}, { 1,  4}, { 2,  3}, { 3,  5}, { 3,  6},
      { 4,  0}, { 5,  8}, { 6,  1}, { 6,  4}, { 7,  0}, { 7,  1},
      { 7,  8}, { 9,  4},
    }
  }},
  { { 6, 14 }, Map{ 5, { 6 },
    {
       5,  0,  1,  3,  2,  3,  6,  5,  0,  0,
    }, {
      { 0,  4}, { 0,  6}, { 1,  0}, { 1,  5}, { 2,  4}, { 3,  7},
      { 4,  2}, { 5,  0}, { 5,  1}, { 5,  4}, { 6,  4}, { 7,  9},
      { 8,  4}, { 8,  6}, { 9,  8},
    }
  }},
  { { 2, 15 }, Map{ 1, { 2 },
    {
       1,  0,  6,  0,  0,  0,  5,  0,  5,  4,
    }, {
      { 1,  3}, { 1,  7}, { 3,  2}, { 3,  7}, { 3,  9}, { 4,  5},
      { 4,  7}, { 5,  3}, { 5,  8}, { 7,  3}, { 7,  8}, { 8,  1},
      { 8,  6}, { 9,  5}, { 9,  8},
    }
  }},
  { { NO_ROOM, 0 }, Map{ 9, { 3 },
    {
       1,  4,  2,  3,  0,  1,  0,  4,  1,  0,
    }, {
      { 0,  1}, { 1,  0}, { 1,  5}, { 1,  9}, { 2,  0}, { 2,  5},
      { 2,  7}, { 3,  9}, { 4,  9}, { 6,  2}, { 6,  3}, { 6,  5},
      { 7,  0}, { 7,  2}, { 7,  3}, { 7,  8}, { 8,  2}, { 9,  4},
    }
  }},
};

int main() {
  for (const auto& [ exp_res,  M ] : test_data) {
    Result stud_res = find_lost_cat(M);
    if (! (stud_res == exp_res))
    {
        std::cout << "Fail: " << exp_res << " != " << stud_res << std::endl;
    }
  }

  return 0;
}

#endif

