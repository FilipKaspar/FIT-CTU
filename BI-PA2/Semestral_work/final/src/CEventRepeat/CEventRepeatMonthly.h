#ifndef CCALENDARMONTH_CPP_CEVENTREPEATMONTHLY_H
#define CCALENDARMONTH_CPP_CEVENTREPEATMONTHLY_H

#include "CEventRepeat.h"

class CEventRepeatMonthly : public CEventRepeat{
public:
    std::string getTextRepeat() const override;
    bool checkRepeatInDay(const tm &user_date) const override;
    int getSaveNumber() const override;
};

#endif
