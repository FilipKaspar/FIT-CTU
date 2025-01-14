#ifndef CCALENDARMONTH_CPP_CEVENTREPEATNONE_H
#define CCALENDARMONTH_CPP_CEVENTREPEATNONE_H

#include "CEventRepeat.h"

class CEventRepeatNone : public CEventRepeat{
public:
    std::string getTextRepeat() const override;
    bool checkRepeatInDay(const tm &user_date) const override;
    int getSaveNumber() const override;
};

#endif
