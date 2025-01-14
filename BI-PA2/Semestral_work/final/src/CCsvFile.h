#ifndef MAIN_CPP_CCSVFILE_H
#define MAIN_CPP_CCSVFILE_H

#include "CRecords.h"
#include "GeneralFuncs.h"

#include <iostream>
#include <fstream>
#include <string>
#include <set>

/**
 * Class that handles exporting and importing events.
 */
class CCsvFile{
public:
    /**
     * Constructor open file for writing.
     * @param file_Name Name of the file user wish to open.
     * @param delimiter Delimiter used to separate cells in .csv files.
     */
    explicit CCsvFile(const std::string & file_Name, const char & delimiter = ',');
    /**
     * Constructor open file for reading.
     * @param file_Name Name of the file user wish to open.
     * @param useless   Useless int to differentiate between opening for
     * writing and for reading.
     * @param delimiter Delimiter used to separate cells in .csv files.
     */
    CCsvFile(const std::string & file_Name, const int & useless, const char & delimiter = ',');
    ~CCsvFile();

    /**
     * Export all the events to user specified file.
     * @param records Events to export.
     */
    void transferToFile(const CRecords::recordSet & records);
    /**
     * Import all the events from user specified file.
     * @param records Events to import.
     */
    void transferFromFile(CRecords & records);
    /**
     * Checks if file is being open. Csv file are not supporting writing
     * from third party application while being opened.
     * @return True if is NOT being opened.
     */
    bool checkFileOpened();
private:
    /**
     * Object on which file is gonna be open.
     */
    std::fstream fs;
    /**
     * Delimiter set by the user or default: ",".
     */
    char delimiter;
};

#endif