#include "CCalendarWeek.h"

using namespace std;

CCalendarWeek::CCalendarWeek() : CCalendar("Week") {
    recordsToPrint.resize(7);
    for(int i = 0; i < 7; i++) recordsToPrint[i] = CRecords::recordSet(compareFunc);
}
CCalendarWeek::~CCalendarWeek() = default;

struct tm CCalendarWeek::getSelectedRecords(const CRecords &all_records) {
    tm user_date = getDateFromUser();
    tm end_date = user_date;
    end_date.tm_mday += 7;
    findRecords(user_date, end_date, all_records);

    return user_date;
}

void CCalendarWeek::printCalendar(tm & user_date) const {
    string to_show;
    const int FRAME_LENGTH = 225;
    const int GAPS = 12;

    tm date1 = user_date;
    date1.tm_mday += 6;
    time_t time = timegm(&date1);
    date1 = *gmtime(&time);

    cout << "+" << right << setfill('-')<< setw( FRAME_LENGTH) << '+'<< endl;
    to_show = getMonth(user_date.tm_mon) + " " + to_string(user_date.tm_mday) + " " + to_string(user_date.tm_year + 1900)
              + " - " + getMonth(date1.tm_mon) + " " + to_string(date1.tm_mday) + " " + to_string(date1.tm_year + 1900);
    cout << "| " << left << setfill(' ') << setw( FRAME_LENGTH - 2) << to_show << '|' << endl;
    cout << "+" << right << setfill('-') << setw( FRAME_LENGTH) << '+'<< endl;
    to_show = "";
    for(int i = 0; i < 7; i++) {
        int day = (user_date.tm_wday + i) % 7;
        to_show += "|";
        for (int k = 0; k < GAPS; k++) {
            to_show += " ";
        }
        to_show += getDay(day);
        for (int k = 0; k < GAPS; k++) {
            to_show += " ";
        }
    }
    cout << left << setfill(' ') << setw( FRAME_LENGTH) << to_show << '|' << endl;
    cout << "+" << right << setfill('-')<< setw( FRAME_LENGTH) << '+'<< endl;

    int count = 0;
    vector<int> pos(7);
    bool done = true;
    tm tmp{};

    while(true) {
        tmp = user_date;
        stringstream ss;
        ss << "| " << flush;
        for (int i = 0; i < 7; i++) {
            if (next(recordsToPrint[i].begin(), pos[i]) == recordsToPrint[i].end()) {
                ss << right << setfill(' ') << setw(GAPS * 2 + int(getDay(tmp.tm_wday).size()) + 1) << " | " << flush;
            } else {
                ss << left << setfill(' ') << setw(GAPS * 2 + int(getDay(tmp.tm_wday).size()) - 2) << (*next(recordsToPrint[i].begin(), pos[i]))->name << " | " << flush;
                pos[i]++;
                count++;
                done = false;
            }
            tmp.tm_mday += 1;
            timegm(&tmp);
        }
        if(done) break;
        done = true;
        cout << ss.str() << endl;
    }

    cout << "+" << right << setfill('-')<< setw( FRAME_LENGTH) << '+'<< endl;
    to_show = "Total amount of events: " + to_string(count);
    cout << "| " << left << setfill(' ') << setw( FRAME_LENGTH - 2) << to_show << '|' << endl;
    cout << "+" << right << setfill('-')<< setw( FRAME_LENGTH) << '+'<< endl;
}

