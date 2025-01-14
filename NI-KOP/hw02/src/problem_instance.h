#ifndef INSTANCE_H
#define INSTANCE_H

#include <vector>
#include "clause.h"

using namespace std;

class ProblemInstance {
public:
    vector<int> weights;
    vector<Clause> clauses;
};

#endif //INSTANCE_H
