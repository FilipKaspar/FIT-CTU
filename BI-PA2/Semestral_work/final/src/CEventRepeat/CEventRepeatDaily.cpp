#include "CEventRepeatDaily.h"

std::string CEventRepeatDaily::getTextRepeat() const {
    return "Daily";
}

bool CEventRepeatDaily::checkRepeatInDay(const tm &user_date) const {
    return true;
}

int CEventRepeatDaily::getSaveNumber() const {
    return 1;
}

