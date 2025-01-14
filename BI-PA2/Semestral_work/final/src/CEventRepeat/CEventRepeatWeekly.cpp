#include "CEventRepeatWeekly.h"

std::string CEventRepeatWeekly::getTextRepeat() const {
    return "Weekly";
}

bool CEventRepeatWeekly::checkRepeatInDay(const tm &user_date) const {
    return event_start_date.tm_wday == user_date.tm_wday;
}

int CEventRepeatWeekly::getSaveNumber() const {
    return 2;
}

