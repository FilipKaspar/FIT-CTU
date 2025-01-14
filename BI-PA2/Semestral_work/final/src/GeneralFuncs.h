#ifndef MAIN_CPP_GENERALFUNCS_H
#define MAIN_CPP_GENERALFUNCS_H

#include "CRecords.h"
#include <string>

/**
 * Main switch that decide which function to call after user action is
 * selected.
 * @param user_action By user selected action.
 * @param records On which recordsToPrint object is function gonna be executed.
 * @return True when anything else than "quit" is selected.
 */
bool performAction(const char & user_action, CRecords & records);
/**
 * Convert month from numerical view to letters.
 * @param month Month to be converted.
 * @return String representing the month in letters.
 */
std::string getMonth(const int & month);
/**
 * Convert day from numerical view to letters.
 * @param day Day to be converted.
 * @return String representing the day in letters.
 */
std::string getDay(const int & day);

/**
 * Check if inputted date and time is correct (within calendar boundaries).
 * @param Date Date to be checked.
 * @return True if NOT within correct boundaries.
 */
bool checkTime(const tm & Date);
/**
 * Check if inputted date and time is correct while considering leap year.
 * @param Date Date to be checked.
 * @return True if NOT leap year.
 */
bool checkLeapYear(const tm & Date);
/**
 * Calculate the time between 2 dates.
 * @param start Start date.
 * @param end End date.
 * @return Time between 2 dates.
 */
tm calculateDuration(tm & start, tm & end);
/**
 * Checks if new record that is being created isn't making time collision with another one.
 * @param records Records already created.
 * @param new_record New record that is being checked.
 * @return True if collision with another event was found. False otherwise.
 */
bool checkCollisions(CRecords & records, CRecords::CRecord & new_record, CRecords::CRecord & col_record);
/**
 * Converts time from tm struct to int time.
 * @param time Time in tm struct.
 * @return Time in second.
 */
size_t convertToSeconds(tm & time);
/**
 * Checks if two dates overlap in terms of their times.
 * @param cur_record_time_start Start of cur_record.
 * @param cur_record_time_end End of cur_record.
 * @param new_record_time_start Start of new_record.
 * @param new_record_time_end End of new_record.
 * @return True if new_record is within the time of cur_record. False otherwise.
 */
bool checkOverlap(size_t cur_record_time_start, size_t cur_record_time_end, size_t new_record_time_start, size_t new_record_time_end);


#endif
