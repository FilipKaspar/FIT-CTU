#ifndef MAIN_CEVENTREPEATDAILY_H
#define MAIN_CEVENTREPEATDAILY_H

#include "CEventRepeat.h"

class CEventRepeatDaily : public CEventRepeat{
public:
    std::string getTextRepeat() const override;
    bool checkRepeatInDay(const tm &user_date) const override;
    int getSaveNumber() const override;
};

#endif
