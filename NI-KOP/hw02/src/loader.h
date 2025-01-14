#ifndef LOADER_H
#define LOADER_H

#include <filesystem>
#include "problem_instance.h"

namespace fs = std::filesystem;

using namespace std;

ProblemInstance parseDIMACS(const fs::directory_entry & filename);

#endif //LOADER_H
