#ifndef MAIN_CPP_CCALENDARWEEK_H
#define MAIN_CPP_CCALENDARWEEK_H

#include "CCalendar.h"
#include "../GeneralFuncs.h"
#include "../UserInput.h"

/**
 * Class for handling calendar week view.
 */
class CCalendarWeek : public CCalendar{
public:
    explicit CCalendarWeek();
    ~CCalendarWeek() override;

    void printCalendar(tm &user_date) const override;
    struct tm getSelectedRecords(
            const CRecords &all_records) override;
};

#endif
