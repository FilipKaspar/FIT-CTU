#ifndef MAIN_CPP_CCALENDARMONTH_H
#define MAIN_CPP_CCALENDARMONTH_H

#include "CCalendar.h"
#include "../GeneralFuncs.h"
#include "../UserInput.h"

/**
 * Class for handling calendar monthly view.
 */
class CCalendarMonth : public CCalendar{
public:
    explicit CCalendarMonth();
    ~CCalendarMonth() override;

    void printCalendar(tm &user_date) const override;
    struct tm getSelectedRecords(
            const CRecords &all_records) override;
};

#endif
