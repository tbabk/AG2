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

using namespace std;

void DFS_fill_Z(size_t actual_node, std::deque<size_t> & Z, const std::vector<Corridor> & corridors, std::vector<bool> & visited)
{
  visited.at(actual_node) = true;
  for (const auto & corridor: corridors)
  {
    if (corridor.second == actual_node && (!visited.at(corridor.first)))
    {     
      DFS_fill_Z(corridor.first, Z, corridors, visited);
    }
  }
  Z.push_front(actual_node);
}

void DFS_assign_components(size_t actual_node, const std::vector<Corridor> & corridors, std::vector<int> & nodes_component)
{
  for (const auto & corridor: corridors)
  {
    if (corridor.first == actual_node && nodes_component.at(corridor.second) == -1)
    {
      nodes_component.at(corridor.second) = nodes_component.at(corridor.first);
      DFS_assign_components(corridor.second, corridors, nodes_component);
    }
  }
}

void StronglyConnectedComponents(std::vector<int> & nodes_component, const std::vector<Corridor> & corridors, size_t & amount_of_components)
{
  std::deque<size_t> Z;
  std::vector<bool> visited = std::vector<bool>(nodes_component.size(), false);

  for (size_t i = 0; i < nodes_component.size(); i++)
  {
    if (! visited.at(i))
    {
      DFS_fill_Z(i, Z, corridors, visited);
    }
  }

  while (! Z.empty())
  {
    size_t actual = Z.front();
    Z.pop_front();
    if (nodes_component.at(actual) == -1) // undefined 
    { 
      nodes_component.at(actual) = amount_of_components++;
      DFS_assign_components(actual, corridors, nodes_component);
    }
  }
}



void find_lost_cat(const Map& map) {
  // TODO

  std::vector<int> nodes_component = std::vector<int>(map.food.size(), -1);
  size_t amount_components = 0;

  StronglyConnectedComponents(nodes_component, map.corridors, amount_components);
  
  cout << "amount of components: " <<  amount_components << endl;
  cout << "components: " << endl;

  for(size_t i = 0; i < nodes_component.size(); i++)
  {
    cout << "node: " << i << ", component: " << nodes_component.at(i) << endl;
  }

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

  find_lost_cat(Map{ 0, { 1 },
    { 0, 1, 1, 2, 1 },
    {
      { 0, 2 }, { 0, 3 },
      { 2, 4 }, { 3, 4 },
      { 0, 1 }, { 4, 1 }
    }
  });
  
  
  // { { 1, 4 }, Map{ 0, { 1 },
  //   { 0, 1, 1, 2, 1 },
  //   {
  //     { 0, 2 }, { 0, 3 },
  //     { 2, 4 }, { 3, 4 },
  //     { 0, 1 }, { 4, 1 }
  //   }
  // }}


  // for (const auto& [ exp_res,  M ] : test_data) {
  //   Result stud_res = find_lost_cat(M);
  //   if (! (stud_res == exp_res))
  //   {
  //       std::cout << "Fail: " << exp_res << " != " << stud_res << std::endl;
  //   }
  // }

  return 0;
}

#endif


