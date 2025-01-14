#ifndef CCALENDARMONTH_CPP_CEVENTREPEATWEEKLY_H
#define CCALENDARMONTH_CPP_CEVENTREPEATWEEKLY_H

#include "CEventRepeat.h"

class CEventRepeatWeekly : public CEventRepeat{
public:
    std::string getTextRepeat() const override;
    bool checkRepeatInDay(const tm &user_date) const override;
    int getSaveNumber() const override;
};

#endif
