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

void DFS_fill_Z(size_t actual_node, std::deque<size_t> & Z, const std::vector<std::vector<size_t>> & neighbours, std::vector<bool> & visited)
{
  deque<size_t> helper;

  helper.push_front(actual_node);

  visited.at(actual_node) = true;

  while (! helper.empty())
  {
    bool all_visited = true;
    size_t actual = helper.front();
    for (const auto & neighbour: neighbours.at(actual))
    {
      if (! visited.at(neighbour))
      {
        visited.at(neighbour) = true;
        all_visited = false;
        helper.push_front(neighbour);
        break;
      }
    }
    if (all_visited)
    {
        helper.pop_front();
        Z.push_front(actual);
    }
  }
}

void DFS_assign_components(size_t actual_node, const std::vector<std::vector<size_t>> & neighbours, std::vector<int> & nodes_component)
{
  queue<size_t> que;
  que.push(actual_node);

  while(! que.empty())
  {
    size_t front = que.front();
    que.pop();
    for (const auto & neighbour : neighbours.at(front))
    {
      if (nodes_component.at(neighbour) == -1)
      {
        nodes_component.at(neighbour) = nodes_component.at(front);
        que.push(neighbour);
      }
    }
  }
}

void StronglyConnectedComponents(std::vector<int> & nodes_component, const std::vector<Corridor> & corridors, size_t & amount_of_components)
{
  std::deque<size_t> Z;
  std::vector<bool> visited = std::vector<bool>(nodes_component.size(), false);
  std::vector<std::vector<size_t>> neighbours = std::vector<std::vector<size_t>>(nodes_component.size());
  std::vector<std::vector<size_t>> neighbours_trans = std::vector<std::vector<size_t>>(nodes_component.size());

  for (const auto & corridor: corridors)
  {
    neighbours.at(corridor.first).push_back(corridor.second);
    neighbours_trans.at(corridor.second).push_back(corridor.first);
  }

  for (size_t i = 0; i < nodes_component.size(); i++)
  {
    if (! visited.at(i))
    {
      DFS_fill_Z(i, Z, neighbours_trans, visited);
    }
  }

  while (! Z.empty())
  {
    size_t actual = Z.front();
    Z.pop_front();
    if (nodes_component.at(actual) == -1) // undefined 
    { 
      nodes_component.at(actual) = amount_of_components++;
      DFS_assign_components(actual, neighbours, nodes_component);
    }
  }
}

void DFS_components(size_t starting_component, std::vector<std::pair<int, int>> & components, const std::vector<std::pair<size_t, size_t>> & inter_component_paths)
{
  std::queue<size_t> que;
  que.push(starting_component);
  components.at(starting_component).second = components.at(starting_component).first;

  while(! que.empty())
  {
    size_t actual = que.front();
    que.pop();

    for (const auto & int_cmp_paths: inter_component_paths)
    {
      if (int_cmp_paths.first == actual && (components.at(int_cmp_paths.first).second + components.at(int_cmp_paths.second).first) > components.at(int_cmp_paths.second).second)
      {
        components.at(int_cmp_paths.second).second = (components.at(int_cmp_paths.first).second + components.at(int_cmp_paths.second).first);
        que.push(int_cmp_paths.second);
      }
    }
  }  
  
}


Result find_lost_cat(const Map& map) {
  std::vector<int> nodes_component = std::vector<int>(map.food.size(), -1);
  size_t amount_components = 0;

  StronglyConnectedComponents(nodes_component, map.corridors, amount_components);
  std::vector<std::pair<int, int>> components = std::vector<std::pair<int, int>>(amount_components, {0, -1});

  for (size_t i = 0; i < nodes_component.size(); i++)
  {
    components.at(nodes_component.at(i)).first += map.food.at(i);
  }

  std::vector<std::vector<bool>> helper_matrix_connection_added = std::vector<std::vector<bool>>(components.size(), std::vector<bool>(components.size(), false));
  std::vector<std::pair<size_t, size_t>> inter_component_paths;

  // add connections between the components
  for (size_t i = 0; i < map.corridors.size(); i++)
  {
    size_t node1 = map.corridors.at(i).first;
    size_t node2 = map.corridors.at(i).second;

    // if they are in the different component and there is not yet a edge between those components add an edge now
    size_t comp_node1 = nodes_component.at(node1);
    size_t comp_node2 = nodes_component.at(node2);


    if (comp_node1 != comp_node2 && (!helper_matrix_connection_added.at(comp_node1).at(comp_node2)))
    {
      helper_matrix_connection_added.at(comp_node1).at(comp_node2) = true; // signalize that an edge has been added between those components
      inter_component_paths.push_back({comp_node1, comp_node2}); 
    }
  }


  DFS_components(nodes_component.at(map.s), components, inter_component_paths);

  unsigned bed_taken = 0;
  int food_eaten = -1;

  for (const auto & bed: map.beds)
  {
    if (components.at(nodes_component.at(bed)).second > food_eaten)
    {
      bed_taken = bed;
      food_eaten = components.at(nodes_component.at(bed)).second;
    }
  }

  if (food_eaten == -1)
  {
    return Result{NO_ROOM, 42};
  }

  unsigned food_eaten_conv = (unsigned) food_eaten;

  return Result{bed_taken, food_eaten_conv};
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


