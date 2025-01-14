#include "CEventRepeatNone.h"
#include "CEventRepeatDaily.h"
#include "CEventRepeatWeekly.h"
#include "CEventRepeatMonthly.h"
#include "CEventRepeatTwoWeeks.h"
#include "CEventRepeat.h"

using namespace std;

const unordered_map<int, function<shared_ptr<CEventRepeat>()>> CEventRepeat::repeats = {
        {0, []() { return make_shared<CEventRepeatNone>(); } },
        {1, []() { return make_shared<CEventRepeatDaily>(); } },
        {2, []() { return make_shared<CEventRepeatWeekly>(); } },
        {3, []() { return make_shared<CEventRepeatMonthly>(); } },
        {4, []() { return make_shared<CEventRepeatTwoWeeks>(); } },
};

void CEventRepeat::addDate(const tm & start_date, const tm & end_date) {
    this->event_start_date = start_date;
    this->event_end_date = end_date;
}