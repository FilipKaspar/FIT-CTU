#include <iostream>
#include <fstream>
#include "loader.h"
#include "set_handler.h"
#include "problem_instance.h"
#include "simulated_annealing.h"

using namespace std;

int fileToSave = 0;

SetHandler::SetHandler(const fs::directory_entry& setFilePath, const int & repeatFile, const int & testFiles, const string & outputFileName) {
    this->setName = setFilePath.path().filename().string();
    this->setPathName = setFilePath.path().string();
    this->repeatFile = repeatFile;
    this->testFiles = testFiles;
    this->outputFileName = outputFileName;
}

void SetHandler::averageSet() {
    this->averageSuccessRate = this->averageSuccessRate / (this->repeatFile * this->testFiles);
    this->averageIterations = this->averageIterations / (this->repeatFile * this->testFiles);
    this->averageRelativeError = this->averageRelativeError / (this->repeatFile * this->testFiles);
}

int SetHandler::getInstanceOptimumWeight(const fs::directory_entry& instanceFilePath) const {
    string fileName = this->setPathName + "-opt.dat";
    ifstream file(fileName);
    if (!file.is_open()) {
        cerr << "Error: Could not open file " << fileName << endl;
        exit(1);
    }

    string instanceNameInFile = instanceFilePath.path().filename().string();
    instanceNameInFile = instanceNameInFile.substr(1); // w is missing as well for some reason
    size_t extensionPos = instanceNameInFile.find_last_of('.');
    if (extensionPos != string::npos) {
        instanceNameInFile = instanceNameInFile.substr(0, extensionPos);
    }

    vector<string> firstColumn;
    vector<string> lines;

    string line;
    while (getline(file, line)) {
        lines.push_back(line);
        istringstream iss(line);
        string firstWord;
        iss >> firstWord;
        firstColumn.push_back(firstWord);
    }

    for (size_t i = 0; i < firstColumn.size(); ++i) {
        if (firstColumn[i] == instanceNameInFile) {
            istringstream iss(lines[i]);
            string word, secondWord;
            iss >> word;
            if (iss >> secondWord) {
                return stoi(secondWord);
            }
            throw logic_error("No second word on the line.");
        }
    }

    throw logic_error("The target \"" + instanceNameInFile + "\" was not found in the first column.");
}

void SetHandler::processInstance(const fs::directory_entry& instanceFilePath, const int & fileCountInSet) {
    ProblemInstance instance = parseDIMACS(instanceFilePath);
    double instanceAverageWeight = 0;

    for (int i = 0; i < repeatFile; i++) {
        cout << "File repeat number: " << i + 1 << endl;
        SimulatedAnnealing annealing = SimulatedAnnealing(instance, "instances_data/" + this->setName + "-" + instanceFilePath.path().filename().string());
        if (fileCountInSet == fileToSave && i == 0) annealing.setSaveOption(true);
        vector<bool> finalConfiguration = annealing.solve_annealing();

        const int finalWeight = annealing.getSolutionWeight(finalConfiguration);


        instanceAverageWeight += finalWeight;
        this->averageIterations += annealing.getIterations();
        if (finalWeight > 0) this->averageSuccessRate += 1;

        cout << "Best solution weight: " << finalWeight << endl;
        cout << "Solution: ";
        for (const int var : finalConfiguration) {
            cout << var << " ";
        }
        cout << endl;
    }
    instanceAverageWeight /= repeatFile;
    this->averageWeight += instanceAverageWeight;

    int instanceOptimumWeight = getInstanceOptimumWeight(instanceFilePath);
    this->averageOptimumWeight += instanceOptimumWeight;

    double relativeError = abs(instanceAverageWeight - instanceOptimumWeight) / instanceOptimumWeight;
    this->averageRelativeError += relativeError;
    if (relativeError > this->maxRelativeError) this->maxRelativeError = relativeError;
}

void SetHandler::saveSetResults() const {
    ofstream file(this->outputFileName, ios::app);
    if (file.is_open()) {
        file << this->setName << ", " << flush;
        file << this->averageSuccessRate << ", " << flush;
        file << this->averageIterations << ", " << flush;
        file << this->averageWeight << ", " << flush;
        file << this->averageOptimumWeight << ", " << flush;
        file << this->averageRelativeError << ", " << flush;
        file << this->maxRelativeError << endl;
    } else throw logic_error("Could not open file: " + this->outputFileName);
}


