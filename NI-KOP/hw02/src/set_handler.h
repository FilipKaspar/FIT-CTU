#ifndef INSTANCE_HANDLER_H
#define INSTANCE_HANDLER_H

#include <string>
#include <filesystem>

using namespace std;
namespace fs = std::filesystem;

class SetHandler {
public:
    explicit SetHandler(const fs::directory_entry& setFilePath, const int & repeatFile, const int & testFiles, const string & outputFileName);

    void processInstance(const fs::directory_entry& instanceFilePath, const int & fileCountInSet);

    void saveSetResults() const;

    void averageSet();

    int getInstanceOptimumWeight(const fs::directory_entry& instanceFilePath) const;

private:
    string setName;
    string setPathName;
    string outputFileName;
    int repeatFile;
    int testFiles;

    double averageWeight = 0; // average per instance added together
    double averageOptimumWeight = 0;
    double averageSuccessRate = 0;
    double averageRelativeError = 0;
    double maxRelativeError = 0;
    double averageIterations = 0;
};

#endif //INSTANCE_HANDLER_H
