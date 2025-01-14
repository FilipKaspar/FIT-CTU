#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <string>
#include <random>
#include <chrono>

#include "simulated_annealing.h"

SimulatedAnnealing::SimulatedAnnealing(const ProblemInstance& instance, string outputFileName) {
    this->instance = instance;
    this->setTemperature();

    size_t extensionPos = outputFileName.find_last_of('.');
    if (extensionPos != string::npos) {
        outputFileName = outputFileName.substr(0, extensionPos);
    }
    this->outputFileName = outputFileName + ".csv";
}

void SimulatedAnnealing::setTemperature() {
    int tempInitialTemp = 0;
    for (const int & weight : this->instance.weights) {
        tempInitialTemp += weight;
    }
    this->initialTemp = tempInitialTemp * initTemperatureMultiplier;
}

void SimulatedAnnealing::setSaveOption(const bool & saveOption) {
    this->saveIteration = saveOption;
}


vector<Clause> SimulatedAnnealing::getUnsolvedClauses(const vector<bool>& solution) {
    vector<Clause> unsolved;
    bool satisfied = false;
    for (const auto& clause : this->instance.clauses) {
        satisfied = false;
        for (const int & literal : clause.literals) {
            if ((literal > 0 && solution[abs(literal) - 1] == true) ||
                (literal < 0 && solution[abs(literal) - 1] == false)) {
                satisfied = true;
                break;
                }
        }
        if (!satisfied) {
            unsolved.push_back(clause);
        }
    }
    return unsolved;
}

int SimulatedAnnealing::getAmountSolvedClauses(const vector<bool>& solution) {
    vector<Clause> unsolvedClauses = getUnsolvedClauses(solution);
    return this->instance.clauses.size() - unsolvedClauses.size();
}

int SimulatedAnnealing::getSolutionWeight(const vector<bool>& solution) {
    const int solvedClauses = getAmountSolvedClauses(solution);

    if (solvedClauses == this->instance.clauses.size()) {
        int totalWeight = 0;
        for (int i = 0; i < solution.size(); i++) {
            if (solution[i]) totalWeight += this->instance.weights[i];
        }
        return totalWeight;
    }
    return solvedClauses - static_cast<int>(this->instance.clauses.size());
}

bool SimulatedAnnealing::frozen(const double temperature) const {
    return temperature <= this->frozenThreshold ;
}

bool SimulatedAnnealing::equilibrium(const int iterations) const {
    return iterations >= this->maxEquilibriumIterations;
}

vector<bool> SimulatedAnnealing::getStartingConfiguration() const {
    mt19937 rng(chrono::high_resolution_clock::now().time_since_epoch().count());
    uniform_int_distribution<int> distribution(0, 1);

    vector<bool> startingConfiguration(this->instance.weights.size());
    for (auto && var : startingConfiguration) {
        var = static_cast<bool>(distribution(rng));
    }

    return startingConfiguration;
}

vector<bool> SimulatedAnnealing::getNextState(const vector<bool> & oldConfiguration) {
    vector<bool> newConfiguration = oldConfiguration;

    mt19937 rng(chrono::high_resolution_clock::now().time_since_epoch().count());
    uniform_int_distribution<mt19937::result_type> distributionVars(0, static_cast<int>(oldConfiguration.size() - 1));

    uniform_int_distribution<mt19937::result_type> distributionPercentage(0, 100);

    const vector<Clause>unsolvedClauses = getUnsolvedClauses(oldConfiguration);
    int varToFlip = distributionVars(rng);
    if (!unsolvedClauses.empty()) {
        uniform_int_distribution<mt19937::result_type> distributionClauses(0, static_cast<int>(unsolvedClauses.size() - 1));
        Clause chosenClause = unsolvedClauses[distributionClauses(rng)];

        uniform_int_distribution<mt19937::result_type> distributionLiterals(0, static_cast<int>(chosenClause.literals.size() - 1));
        varToFlip = abs(chosenClause.literals[distributionLiterals(rng)]) - 1;
    }
    newConfiguration[varToFlip] = !newConfiguration[varToFlip]; // flips the var value

    return newConfiguration;
}

int SimulatedAnnealing::getIterations() const {
    return this->totalIterations;
}

void SimulatedAnnealing::saveIterationWeight(const int & weight) const {
    ofstream file(this->outputFileName, ios::app);
    if (file.is_open()) {
        file << this->totalIterations << ", " << flush;
        file << weight << endl;
    }
}


vector<bool> SimulatedAnnealing::solve_annealing() {
    vector<bool> currentConfiguration = getStartingConfiguration();
    vector<bool> bestConfiguration = currentConfiguration;

    mt19937 rng(chrono::high_resolution_clock::now().time_since_epoch().count());
    uniform_int_distribution<mt19937::result_type> distribution(0, 1);
    double temperature = this->initialTemp;
    int equilibriumIterations = 0;

    while (!frozen(temperature)) {
        while (!equilibrium(equilibriumIterations)) {
            vector<bool> nextNeighborConfiguration = getNextState(currentConfiguration);
            const double delta =  getSolutionWeight(currentConfiguration) - getSolutionWeight(nextNeighborConfiguration);

            if (getSolutionWeight(nextNeighborConfiguration) > getSolutionWeight(currentConfiguration) ||
                distribution(rng) < exp(-delta / temperature)) {
                if (getSolutionWeight(nextNeighborConfiguration) > getSolutionWeight(bestConfiguration)) {
                    bestConfiguration = nextNeighborConfiguration;
                }
                currentConfiguration = nextNeighborConfiguration;
            }
            if (this->saveIteration && this->outputFileName.find('M') != std::string::npos && getSolutionWeight(currentConfiguration) > 0)
                this->saveIterationWeight(getSolutionWeight(currentConfiguration));
            equilibriumIterations++;
            this->totalIterations++;
        }

        temperature *= this->coolingRate;
        equilibriumIterations = 0;
    }

    cout << "Temperature: " << temperature << endl;
    cout << "Iterations: " << this->totalIterations << endl;
    return bestConfiguration;
}