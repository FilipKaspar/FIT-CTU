#ifndef MAIN_CPP_CCALENDAR_H
#define MAIN_CPP_CCALENDAR_H


#include <memory>
#include <cmath>
#include "../CRecords.h"
#include "../UserInput.h"

/**
 * Abstract class for classes CCalendarDay, CCalendarWeek, and CCalendarMonth.
 */
class CCalendar{
public:
    /**
     * Set compare function for recordsToPrint multiset and set value to variable "name".
     * @param name Name of the calendar view, "Day", "Week" or "Month".
     */
    explicit CCalendar(const std::string &name);
    virtual ~CCalendar();
    /**
     * Virtual function. Prints calender based on inputted user_date.
     * @param user_date Date to determine which events should be printed.
     */
    virtual void printCalendar(tm & user_date) const = 0;
    /**
     * Select REPEATING events based on user inputted time.
     * @param all_records From where REPEATING events are being selected.
     * @return Date based on events were selected.
     */
    virtual struct tm getSelectedRecords(const CRecords &all_records) = 0;
    /**
     * Select all non-repeating events that meets the user inputted criteria.
     * @param begin_date User inputted time.
     * @param end_date The end time, ex. in day begin_date + 1 day, in week begin_date + 7 days, in month begin_date + 1 mon.
     * @param all_records Non-repeating events to be chose from.
     */
    void findRecords(tm begin_date, tm end_date, const CRecords & all_records);
    /**
     * Main function from where are other functions called.
     * @param records Container from where events will be selected.
     */
    void performActions(const CRecords & records);
    /**
     * Convert date to time_t. Copy because timegm converts to UTC and that does a big neplecha.
     * @param time Tm struct that represent date and time to be converted.
     * @return Converted time_t from tm struct.
     */
    static time_t convertDate(tm time);
    /**
     * Used for checking month repeating events.
     * @param date Date to be checked.
     * @return True if after 1 week there is a new month.
     */
    static bool checkNextMonth(struct tm date);
    /**
     * Compares 2 events by ONLY time.
     * @param a CRecord 1.
     * @param b CRecord 2.
     * @return True when CRecord 2 is chronologically before CRecord 1. Checking only time and not days
     * months or years!
     */
    static bool compareFunc(const std::shared_ptr<CRecords::CRecord> & a, const std::shared_ptr<CRecords::CRecord> & b);

private:
    /**
     * Says if calender is in day, week or month view.
     */
    std::string name;

protected:
    /**
     * All possible events to be displayed.
     */
    std::vector<std::multiset<std::shared_ptr<CRecords::CRecord>, decltype(&compareFunc)>> recordsToPrint;
};

#endif
