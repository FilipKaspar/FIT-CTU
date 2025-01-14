#ifndef __PROGTEST__
#include <cassert>
#include <iostream>
#include <memory>
#include <limits>
#include <optional>
#include <algorithm>
#include <bitset>
#include <list>
#include <array>
#include <vector>
#include <deque>
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <stack>
#include <queue>

using Place = size_t;

struct Map {
    size_t places;
    Place start, end;
    std::vector<std::pair<Place, Place>> connections;
    std::vector<std::vector<Place>> items;
};

template < typename F, typename S >
struct std::hash<std::pair<F, S>> {
    std::size_t operator () (const std::pair<F, S> &p) const noexcept {
        // something like boost::combine would be much better
        return std::hash<F>()(p.first) ^ (std::hash<S>()(p.second) << 1);
    }
};

#endif

class RoomProperties {
public:
    std::vector<Place> hallways;
    std::vector<size_t> items;
};
class House {
public:
    std::vector<RoomProperties> rooms;
    Place start;
    Place end;
    Place num_items;
    Place num_rooms;

    explicit House(const Map & map){
        start = map.start;
        end = map.end;
        num_items = map.items.size();
        num_rooms = map.places;

        rooms.resize(map.places);
        for(const auto & i : map.connections){
            rooms[i.first].hallways.push_back(i.second);
            rooms[i.second].hallways.push_back(i.first);
        }

        Place item = 0;
        for(const auto & i : map.items){
            for(const auto & k : i){
                rooms[k].items.push_back(item);
            }
            item++;
        }
    }
};

void deletePointers(const std::vector<std::vector<std::pair<Place, Place*>>> & predecessors){
    for(const auto & i : predecessors){
        for(const auto & k : i){
            delete k.second;
        }
    }
}

Place calculateMaxDepth(const Place & items){
    Place max_depth = 1;
    for(Place i = 0; i < items; i++){
        max_depth = max_depth * 2;
    }

    return max_depth;
}

Place calculateNewDepth(const std::vector<bool> & collected_items){
    Place max_num = calculateMaxDepth(collected_items.size()-1);
    Place new_depth = 0;

    for(const auto & item : collected_items){
        if(item){
            new_depth += max_num;
        }
        max_num = max_num/2;
    }

    return new_depth;
}

std::list<Place> correctWay(const House & house) {
    if (house.num_rooms == 0) return {};
    std::list<Place> path;
    std::queue<std::pair<Place, Place>> searching_rooms;
    std::vector<std::vector<bool>> already_visited;
    std::vector<std::vector<bool>> already_picked;
    std::vector<bool> temp_items;
    std::vector<std::vector<std::pair<Place, Place>>> predecessors;

    //Making room for all layers of BFS
    Place max_depth = calculateMaxDepth(house.num_items);
    already_visited.resize(max_depth);
    already_picked.resize(max_depth);
    temp_items.resize(house.num_items);
    predecessors.resize(max_depth);
    for (Place i = 0; i < max_depth; i++) {
        already_visited[i].resize(house.num_rooms);
        already_picked[i].resize(house.num_items);
        predecessors[i].resize(house.num_rooms);
    }


    bool found_item = false;
    Place new_depth;
    Place old_depth;

    searching_rooms.push({0, house.start});
    already_visited[0][searching_rooms.front().second] = true;

    predecessors[0][house.start] = std::make_pair(0, -1);

    for (auto i: house.rooms[house.start].items) {
        temp_items[i] = true;
        found_item = true;
    }

    if (found_item) {
        new_depth = calculateNewDepth(temp_items);
        //auto * predecessor = new Place (house.start);
        predecessors[new_depth][house.start] = std::make_pair(0, -1); // predecessor
        already_visited[new_depth][house.start] = true;
        already_picked[new_depth] = temp_items;
        searching_rooms.push({new_depth, house.start});
        searching_rooms.pop();
    }


    // BFS
    while (!searching_rooms.empty()) {

        auto curr_search = searching_rooms.front();
        temp_items = already_picked[curr_search.first];
        new_depth = curr_search.first;

        for (const auto &i: house.rooms[curr_search.second].hallways) {
            found_item = false;
            if (!already_visited[new_depth][i]) {
                std::vector<bool> temp;
                temp = already_picked[new_depth];
                for (auto k: house.rooms[i].items) {
                    if (!already_picked[new_depth][k]) {
                        temp[k] = true;
                        found_item = true;
                    }
                }
                old_depth = new_depth;
                if (found_item) {
                    new_depth = calculateNewDepth(temp);
                }
                if (!already_visited[new_depth][i]) {
                    searching_rooms.push({new_depth, i});
                    already_visited[new_depth][i] = true;
                    already_picked[new_depth] = temp;

                    //auto *predecessor = new Place(curr_search.second);
                    predecessors[new_depth][i] = std::make_pair(old_depth, curr_search.second);
                }
                new_depth = old_depth;
            }
        }

        if((house.num_items == 0 || calculateNewDepth(already_picked[max_depth-1]) == max_depth-1) && already_visited[max_depth-1][house.end]) {
            for (auto room = std::make_pair(max_depth - 1, house.end);
                 room.second != (long unsigned int) -1; room = predecessors[room.first][room.second]) {
                path.push_front(room.second);
            }
            //deletePointers(predecessors);
            return path;
        }
    searching_rooms.pop();
    }
    //deletePointers(predecessors);
    return {};
}

std::list<Place> find_path(const Map &map) {
    // TODO
    House grinch(map);
    auto ret = correctWay(grinch);
    for(auto i : ret){
        std::cout << i << "->";
    };
    std::cout << std::endl;
     return ret;

    //try {
        //return correctWay(grinch);
    //}
    //catch (...){
        //return {};
    //}
}

#ifndef __PROGTEST__

using TestCase = std::pair<size_t, Map>;

// Class template argument deduction exists since C++17 :-)
const std::array examples = {
        TestCase{4, Map{10, 7, 9, {{0, 1}, {0, 6}, {0, 7}, {0, 9}, {1, 3}, {1, 4}, {1, 6}, {1, 7}, {1, 8}, {2, 3}, {2, 9}, {3, 4}, {5, 8}, {5, 9}, {6, 7}, {7, 8}, },{{1, 3, 5, },{2, 3, 6, 8, },{1, 7, },}}},
        TestCase{4, Map{10, 8, 2, {{0, 1}, {0, 2}, {0, 4}, {0, 5}, {0, 7}, {0, 8}, {1, 3}, {1, 7}, {1, 9}, {2, 7}, {2, 8}, {2, 9}, {3, 7}, {4, 5}, {4, 8}, {5, 7}, {7, 9}, },{{2, },{5, 7, },{2, 7, },}}},
        TestCase{3, Map{10, 1, 3, {{0, 3}, {0, 7}, {0, 9}, {1, 3}, {1, 4}, {1, 5}, {1, 9}, {2, 8}, {3, 4}, {3, 6}, {3, 7}, {3, 8}, {4, 5}, {5, 6}, {5, 7}, {6, 9}, {7, 8}, {8, 9}, },{{4, 8, 9, },{3, 5, 8, },{0, 3, 5, 6, },}}},
        TestCase{3, Map{10, 6, 9, {{0, 6}, {0, 7}, {0, 8}, {1, 2}, {1, 3}, {1, 6}, {1, 7}, {2, 4}, {2, 5}, {2, 9}, {3, 6}, {3, 7}, {3, 8}, {4, 7}, {4, 9}, {5, 7}, {5, 9}, {6, 8}, {6, 9}, {8, 9}, },{{0, 1, 2, 4, 8, },{2, 8, 9, },{3, 7, 9, },}}},
        TestCase{5, Map{10, 0, 8, {{0, 1}, {0, 3}, {0, 5}, {0, 9}, {1, 4}, {1, 6}, {2, 3}, {2, 4}, {2, 7}, {3, 6}, {3, 9}, {4, 9}, {5, 6}, {6, 7}, {6, 8}, {6, 9}, {7, 8}, },{{2, 4, },{0, 1, 2, 3, 8, },{8, },}}},
        TestCase{3, Map{10, 6, 5, {{0, 1}, {0, 4}, {0, 6}, {1, 2}, {1, 3}, {1, 5}, {1, 8}, {2, 3}, {2, 4}, {3, 5}, {3, 6}, {4, 5}, {4, 8}, {4, 9}, {5, 7}, {5, 9}, {6, 8}, {7, 9}, },{{1, 5, 9, },{5, },{1, 3, },}}},
};

int main() {
    int fail = 0;
    for (size_t i = 0; i < examples.size(); i++) {
        auto sol = find_path(examples[i].second);
        if (sol.size() != examples[i].first) {
            std::cout << "Wrong anwer for map " << i << std::endl;
            fail++;
        }
    }

    if (fail) std::cout << "Failed " << fail << " tests" << std::endl;
    else std::cout << "All tests completed" << std::endl;

    return 0;
}

#endif
