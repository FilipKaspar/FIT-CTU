#include <vector>
#include <string>
#include <filesystem>
#include <iostream>

#include "set_handler.h"

using namespace std;

namespace fs = std::filesystem;

string dataFolder = "data";
string outputFile = "output.csv";
int testFiles = 50;
int repeatFile = 10;


int main() {
    for (const auto& setDifficultyFolder : fs::directory_iterator(dataFolder)) {
        for (const auto& subsetDifficultyFolder : fs::directory_iterator(setDifficultyFolder)) {
            if (!is_directory(subsetDifficultyFolder)) continue;

            cout << "Currently iterated subset: " << subsetDifficultyFolder << endl;
            SetHandler setHandler = SetHandler(subsetDifficultyFolder, repeatFile, testFiles, outputFile);
            int fileCount = 0;
            for (const auto& instanceFilePath : fs::directory_iterator(subsetDifficultyFolder)) {
                cout << "File tested: " << fileCount + 1 << endl;
                if (fileCount >= testFiles) break;
                if (is_regular_file(instanceFilePath)) {
                    setHandler.processInstance(instanceFilePath, fileCount);
                }
                fileCount++;
            }
            setHandler.averageSet();
            setHandler.saveSetResults();
        }
    }


    return 0;
}