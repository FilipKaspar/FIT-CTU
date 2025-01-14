#ifndef KASPAFIL_CAPPLICATION_H
#define KASPAFIL_CAPPLICATION_H

#include <iostream>
#include <chrono>
#include <ctime>
#include <sstream>
#include <string>
#include <vector>
#include <iomanip>
#include <algorithm>
#include <unistd.h>
#include <set>
#include <memory>
#include <filesystem>

#include "CEventRepeat/CEventRepeat.h"
#include "CEventRepeat/CEventRepeatNone.h"

/**
 * Stores all events in multiset and repeating events in extra multiset.
 */
class CRecords {
public:
    /**
     * Stores Information about every event.
     */
    class CRecord {
    public:
        /**
         * Constructor for event added by importing from a .csv file.
         */
        CRecord() = default;
        /**
         * Constructor for an event added by user.
         * @param useless Useless int just to differentiate between constructors.
         */
        explicit CRecord(const int &useless);
        /**
         * Print details about the event.
         */
        void printRecord();
        /**
         * Convert struct tm to string date and time.
         * @return Time as a string from tm struct.
         */
        static std::string dateToString(tm in_date) ;


        /**
         * Name of the event.
         */
        std::string name;
        /**
         * Date of the event.
         */
        tm dateStartTime{};
        /**
         * End of the event.
         */
        tm dateEndTime{};
        /**
         * Duration of the event. Stores only days, hours, minutes and seconds. Doesn't use mktime or timegm.
         */
        tm duration{};
        /**
         * Location as where the event will be taking place.
         */
        std::string location;
        /**
         * Description for the event.
         */
        std::string description;
        /**
         * People attending the event.
         */
        std::vector<std::string> people{};
        /**
         * Event tags.
         */
        std::vector<std::string> tags{};
        /**
         * How often should event be repeated.
         */
        std::shared_ptr<CEventRepeat> repeats;
    };
    /**
     * Constructor that initializes the containers with custom compare function.
     */
    CRecords();

    /**
     * Add new event. Also add to a corresponding container.
     * @param srcRecord Shared pointer to a CRecord to be added to events.
     */
    bool addRecord(CRecord & srcRecord);
    /**
     * Delete an event.
     * @return True when record has been deleted successfully.
     */
    bool deleteRecord();
    /**
     * Update an event. Delete selected event and create a new one.
     */
    void updateRecord();
    /**
     * Change date of an event.
     */
    void changeDate();
    /**
     * Export events to a .csv file.
     */
    void exportRecords();
    /**
     * Import events from .csv file.
     */
    void importRecords();
    /**
     * Show all events sorted and divided into their months.
     */
    void showRecords() const;

    /**
     * Compares 2 events by date.
     * @param a CRecord 1.
     * @param b CRecord 2.
     * @return True when CRecord 2 is chronologically before CRecord 1.
     */
    static bool compareFunc(const std::shared_ptr<CRecord> & a, const std::shared_ptr<CRecord> & b);
    typedef std::multiset<std::shared_ptr<CRecord>, decltype(&compareFunc)> recordSet;
    /**
     * Show all events that meets the inputted criteria.
     */
    recordSet findRecords();
    /**
     * If more events with the same attributes has been found, this function will be called
     * to select one from them.
     * @return Shared pointer to a specific event.
     */
    std::shared_ptr<CRecord> selectFromRecords() const;
    /**
     * Show with which options user can find events and let him choose one or quit finding.
     * @param records_to_select Selected events based on user inputted criteria.
     * @return Events based on user's input.
     */
    static recordSet findRecordOptions(recordSet &records_to_select, int &user_action) ;
    /**
     * Find events based on name.
     * @param records_to_select Selected events based on user inputted criteria.
     * @return Events based on user's input.
     */
    static recordSet findRecordByName(recordSet &records_to_select);
    /**
     * Find events based on location.
     * @param records_to_select Selected events based on user inputted criteria.
     * @return Events based on user's input.
     */
    static recordSet findRecordByLocation(recordSet &records_to_select);
    /**
     * Find event's based on People attending the event.
     * @param records_to_select Selected events based on user inputted criteria.
     * @return Events based on user's input.
     */
    static recordSet findRecordByPerson(recordSet &records_to_select);
    /**
     * Find event's based on tags in the event.
     * @param records_to_select Selected events based on user inputted criteria.
     * @return Events based on user's input.
     */
    static recordSet findRecordByTag(recordSet &records_to_select);
    /**
     * Find event's that are before user's inputted date.
     * @param records_to_select Selected events based on user inputted criteria.
     * @return Events based on user's input.
     */
    static recordSet findRecordByDateBefore(recordSet &records_to_select);
    /**
     * Find event's that are after user's inputted date.
     * @param records_to_select Selected events based on user inputted criteria.
     * @return Events based on user's input.
     */
    static recordSet findRecordByDateAfter(recordSet &records_to_select);
    /**
     * Find the earliest event from all events. Repetition isn't accounted for.
     * @param records_to_select Selected events based on user inputted criteria.
     * @return Closest event.
     */
    static recordSet findClosestRecord(recordSet &records_to_select);
    /**
     * Checks if there is a time collision with other records.
     * @param new_record New record that wants to be added.
     * @return True if collision was found. Otherwise false.
     */
    bool checkCollision(CRecords::CRecord & new_record);

    /**
     * Return all saved events.
     * @return All saved events.
     */
    const recordSet & getAllRecords() const;
    /**
     * Return all repeating events.
     * @return All repeating events.
     */
    const recordSet & getAllRecordsRepeating() const;
    /**
     * Return all NON repeating events.
     * @return All NON repeating events.
     */
    const recordSet & getAllRecordsNonRepeating() const;
    /**
     * Save events to class variable records.
     */
    void setAllRecords(recordSet in_records);
    /**
     * Map for searching menu.
     */
    const static std::unordered_map<int, std::function<recordSet(recordSet &selectedRecords)>> findOptions;

    friend bool operator==(const CRecord& a, const CRecord& b);

private:
    /**
     * All events.
     */
    recordSet records;
    /**
     * All repeating events.
     */
    recordSet recordsRepeating;
    /**
     * All NON repeating events.
     */
    recordSet recordsNonRepeating;
};
#endif
