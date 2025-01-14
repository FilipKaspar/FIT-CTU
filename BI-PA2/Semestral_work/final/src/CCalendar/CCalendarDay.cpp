#include "CCalendarDay.h"

using namespace std;

CCalendarDay::CCalendarDay() : CCalendar("Day") {
    recordsToPrint.resize(1);
    recordsToPrint[0] = CRecords::recordSet(compareFunc);
}
CCalendarDay::~CCalendarDay() = default;

struct tm CCalendarDay::getSelectedRecords(const CRecords &all_records) {
    tm user_date = getDateFromUser();
    tm end_date = user_date;
    end_date.tm_mday += 1;
    findRecords(user_date, end_date, all_records);

    return user_date;
}
void CCalendarDay::printCalendar(tm & user_date) const {
    string to_show;
    const int FRAME_LENGTH = 95;
    int previous_hour = -5;

    cout << "+" << right << setfill('-')<< setw( FRAME_LENGTH) << '+'<< endl;
    to_show = getMonth(user_date.tm_mon) + " " + to_string(user_date.tm_mday) + " " + to_string(user_date.tm_year + 1900);
    cout << "| " << left << setfill(' ') << setw( FRAME_LENGTH - 2) << to_show << '|' << endl;
    to_show = "Total amount of events: " + to_string(recordsToPrint[0].size());
    cout << "| " << left << setfill(' ') << setw( FRAME_LENGTH - 2) << to_show << '|' << endl;
    cout << "+" << right << setfill('-')<< setw( FRAME_LENGTH) << '+'<< endl;
    for(const auto & i : this->recordsToPrint[0]){
        if(previous_hour != i->dateStartTime.tm_hour){
            to_show = to_string((i->dateStartTime.tm_hour + 1)) + ":00";
            cout << "| " << left << setfill(' ') << setw( FRAME_LENGTH - 2) << to_show << '|'<< endl;
        }
        to_show = to_string(i->dateStartTime.tm_hour + 1) + ":" + to_string(i->dateStartTime.tm_min) + ":" + to_string(i->dateStartTime.tm_sec) + " " + i->name + " - " + i->description;
        cout << "|     " << left << setfill(' ') << setw( FRAME_LENGTH - 6) << to_show <<'|' << endl;
        previous_hour = i->dateStartTime.tm_hour;
    }
    cout << "+" << right << setfill('-')<< setw( FRAME_LENGTH) << '+'<< endl;
}
