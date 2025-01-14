#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <string>
#include <random>
#include <chrono>
#include <filesystem>

#include "clause.h"
#include "problem_instance.h"

namespace fs = std::filesystem;
using namespace std;

ProblemInstance parseDIMACS(const fs::directory_entry & filename) {
    ProblemInstance instance;
    ifstream file(filename.path());
    if (!file.is_open()) {
        cerr << "Error: Could not open file " << filename << endl;
        exit(1);
    }

    int numVariables = 0;
    int numClauses = 0;
    string line;
    while (getline(file, line)) {
        if (line.empty() || line[0] == 'c') {
            continue; // Skip comments
        }
        if (line[0] == 'p') {
            istringstream iss(line);
            string tmp;
            iss >> tmp >> tmp >> numVariables >> numClauses;
        } else if (line[0] == 'w') {
            istringstream iss(line.substr(2)); // Skip "w " prefix
            int weight;
            while (iss >> weight) {
                instance.weights.push_back(weight);
            }
            instance.weights.pop_back(); // Get rid of last zero
        } else {
            istringstream iss(line);
            int literal;
            Clause clause;
            while (iss >> literal && literal != 0) {
                clause.literals.push_back(literal);
            }
            instance.clauses.push_back(clause);
        }
    }

    if (instance.weights.size() != numVariables) {
        cerr << "Error: Mismatch between number of weights" << endl;
        exit(1);
    }

    if (instance.clauses.size() != numClauses) {
        cerr << "Error: Mismatch between number of clauses" << endl;
        exit(1);
    }

    return instance;
}