#ifndef CLAUSE_H
#define CLAUSE_H

#include <vector>

using namespace std;

class Clause {
public:
    vector<int> literals;
    int weight;
};

#endif //CLAUSE_H
