#include "CEventRepeatNone.h"

std::string CEventRepeatNone::getTextRepeat() const {
    return "None";
}

bool CEventRepeatNone::checkRepeatInDay(const tm &user_date) const {
    return false;
}

int CEventRepeatNone::getSaveNumber() const {
    return 0;
}

