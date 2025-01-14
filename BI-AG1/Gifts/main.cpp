#ifndef __PROGTEST__
#include <cassert>
#include <cstdint>
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
#include <random>

using ChristmasTree = size_t;

struct TreeProblem {
    int max_group_size;
    std::vector<uint64_t> gifts;
    std::vector<std::pair<ChristmasTree, ChristmasTree>> connections;
};

#endif

using namespace std;

struct MyTree{
    vector<vector<ChristmasTree>> paths;
    std::vector<uint64_t> gifts;
    vector<uint64_t> guarded;
    vector<uint64_t> unguarded;
};

bool isList(size_t pred,size_t node, MyTree & graph){
    if(graph.paths[node].size() == 1 && graph.paths[node][0] == pred) return true;
    return false;
}

uint64_t maximum(uint64_t a, uint64_t b){
    if(a > b) return a;
    return b;
}

uint64_t rec(size_t pred, size_t node, MyTree & graph, bool guard){
    if(guard){
        if(graph.guarded[node] != uint64_t (-1)) return graph.guarded[node];
    }
    else{
        if(graph.unguarded[node] != uint64_t (-1)) return graph.unguarded[node];
    }
    if(isList(pred, node, graph)){
        if(guard) return graph.gifts[node];
        return 0;
    }
    uint64_t res = 0;
    if(guard){
        res = graph.gifts[node];
        if(graph.guarded[node] != uint64_t (-1)) res += graph.guarded[node];
        else{
            for (auto &i: graph.paths[node]) {
                if(pred == i) continue;
                res += rec(node, i, graph, false);
            }
            graph.guarded[node] = res - graph.gifts[node];
        }

    }
    else{
        if(graph.unguarded[node] != uint64_t (-1)) res += graph.unguarded[node];
        else{
            for (auto &i: graph.paths[node]) {
                if(pred == i) continue;
                res += maximum(rec(node, i, graph, false), rec(node, i, graph, true));
            }
            graph.unguarded[node] = res;
        }
    }
    return res;
}


uint64_t solve(const TreeProblem& tree) {
    // TODO implement

    MyTree graph;
    graph.paths.resize(tree.gifts.size());
    graph.guarded.resize(tree.gifts.size());
    graph.unguarded.resize(tree.gifts.size());
    for(size_t i = 0; i < graph.guarded.size(); i++){
        graph.guarded[i] = -1;
        graph.unguarded[i] = -1;
    }
    graph.gifts = tree.gifts;
    for(auto & i : tree.connections){
        graph.paths[i.first].push_back(i.second);
        graph.paths[i.second].push_back(i.first);
    }

    uint64_t first = rec(-1, 0, graph, true);

    uint64_t second = rec(-1, 0, graph, false);

    return max(first, second);
}

#ifndef __PROGTEST__

using TestCase = std::pair<uint64_t, TreeProblem>;

const std::vector<TestCase> BASIC_TESTS = {
        { 3, { 1, { 1, 1, 1, 2 }, { {0,3}, {1,3}, {2,3} }}},
        { 4, { 1, { 1, 1, 1, 4 }, { {0,3}, {1,3}, {2,3} }}},
        { 57, { 1, {
                    17, 11, 5, 13, 8, 12, 7, 4, 2, 8,
                  }, {
                                    {1, 4}, {6, 1}, {2, 1}, {3, 8}, {8, 0}, {6, 0}, {5, 6}, {7, 2}, {0, 9},
                                  }}},
        { 85, { 1, {
                    10, 16, 13, 4, 19, 8, 18, 17, 18, 19, 10,
                  }, {
                                    {9, 7}, {9, 6}, {10, 4}, {4, 9}, {7, 1}, {0, 2}, {9, 2}, {3, 8}, {2, 3}, {5, 4},
                                  }}},
        { 79, { 1, {
                    8, 14, 11, 8, 1, 13, 9, 14, 15, 12, 1, 11,
                  }, {
                                    {9, 1}, {1, 2}, {1, 4}, {5, 10}, {7, 8}, {3, 7}, {11, 3}, {11, 10}, {6, 8}, {0, 1}, {0, 3},
                                  }}},
        { 102, { 1, {
                    15, 10, 18, 18, 3, 4, 18, 12, 6, 19, 9, 19, 10,
                  }, {
                                    {10, 2}, {11, 10}, {6, 3}, {10, 8}, {5, 3}, {11, 1}, {9, 5}, {0, 4}, {12, 3}, {9, 7}, {11, 9}, {4, 12},
                                  }}},
        { 93, { 1, {
                    1, 7, 6, 18, 15, 2, 14, 15, 18, 8, 15, 1, 5, 6,
                  }, {
                                    {0, 13}, {6, 12}, {0, 12}, {7, 8}, {8, 3}, {12, 11}, {12, 1}, {10, 12}, {2, 6}, {6, 9}, {12, 7}, {0, 4}, {0, 5},
                                  }}},
};

const std::vector<TestCase> BONUS_TESTS = {
        { 3, { 2, { 1, 1, 1, 2 }, { {0,3}, {1,3}, {2,3} }}},
        { 5, { 2, { 1, 1, 1, 4 }, { {0,3}, {1,3}, {2,3} }}},
};

void test(const std::vector<TestCase>& T) {
    int i = 0;
    for (auto &[s, t] : T) {
        if (s != solve(t))
            std::cout << "Error in " << i << " (returned " << solve(t) << ")"<< std::endl;
        i++;
    }
    std::cout << "Finished" << std::endl;
}

int main() {
    test(BASIC_TESTS);
    // test(BONUS_TESTS);
}

#endif


