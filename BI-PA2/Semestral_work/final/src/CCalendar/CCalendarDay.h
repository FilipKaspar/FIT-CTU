#ifndef MAIN_CPP_CCALENDARDAY_H
#define MAIN_CPP_CCALENDARDAY_H

#include "CCalendar.h"
#include "../GeneralFuncs.h"
#include "../UserInput.h"

/**
 * Class for handling calendar day view.
 */
class CCalendarDay : public CCalendar{
public:
    explicit CCalendarDay();
    ~CCalendarDay() override;

    void printCalendar(tm &user_date) const override;
    struct tm getSelectedRecords(
            const CRecords &all_records) override;

    void test();
            
};

#endif
