#ifndef CCALENDARMONTH_CPP_CEVENTREPEATTWOWEEKS_H
#define CCALENDARMONTH_CPP_CEVENTREPEATTWOWEEKS_H

#include "CEventRepeat.h"

class CEventRepeatTwoWeeks : public CEventRepeat{
public:
    std::string getTextRepeat() const override;
    bool checkRepeatInDay(const tm &user_date) const override;
    int getSaveNumber() const override;
};

#endif
