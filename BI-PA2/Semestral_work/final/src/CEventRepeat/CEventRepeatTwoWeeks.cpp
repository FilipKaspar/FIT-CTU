#include "CEventRepeatTwoWeeks.h"

std::string CEventRepeatTwoWeeks::getTextRepeat() const {
    return "Every 14 days";
}

bool CEventRepeatTwoWeeks::checkRepeatInDay(const tm &user_date) const {
    return (event_start_date.tm_wday == user_date.tm_wday && ((user_date.tm_yday / 7) % 2 == (event_start_date.tm_yday / 7) % 2));
}

int CEventRepeatTwoWeeks::getSaveNumber() const {
    return 4;
}

