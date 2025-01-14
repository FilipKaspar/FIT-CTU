#ifndef SIMULATED_ANNEALING_H
#define SIMULATED_ANNEALING_H

#include "clause.h"
#include "problem_instance.h"

using namespace std;

class SimulatedAnnealing {
public:
    explicit SimulatedAnnealing(const ProblemInstance& instance, string outputFileName);

    void setTemperature();

    vector<Clause> getUnsolvedClauses(const vector<bool>& solution);

    int getAmountSolvedClauses(const vector<bool>& solution);

    int getSolutionWeight(const vector<bool>& solution);

    bool frozen(double temperature) const;

    bool equilibrium(int iterations) const;

    vector<bool> getStartingConfiguration() const;

    vector<bool> getNextState(const vector<bool> & oldConfiguration);

    vector<bool> solve_annealing();

    void saveIterationWeight(const int & weight) const;

    int getIterations() const;

    void setSaveOption(const bool & saveOption);
private:
    ProblemInstance instance;
    double initialTemp = 0;
    bool saveIteration = false;
    string outputFileName;

    double initTemperatureMultiplier = 1.2;
    double coolingRate = 0.97;
    int maxEquilibriumIterations = 150;
    double frozenThreshold = 0.15;
    int totalIterations = 0;
};

#endif //SIMULATED_ANNEALING_H
