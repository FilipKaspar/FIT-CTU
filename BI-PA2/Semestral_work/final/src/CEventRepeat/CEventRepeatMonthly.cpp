#include "CEventRepeatMonthly.h"
#include "../CCalendar/CCalendar.h"


std::string CEventRepeatMonthly::getTextRepeat() const {
    return "Monthly";
}

bool CEventRepeatMonthly::checkRepeatInDay(const tm &user_date) const {
    return (event_start_date.tm_wday == user_date.tm_wday && event_start_date.tm_mday / 7 == user_date.tm_mday / 7)
           || (event_start_date.tm_wday == user_date.tm_wday && CCalendar::checkNextMonth(user_date) && CCalendar::checkNextMonth(event_start_date));
}

int CEventRepeatMonthly::getSaveNumber() const {
    return 3;
}

