#include "CCalendarMonth.h"

using namespace  std;

CCalendarMonth::CCalendarMonth() : CCalendar("Month") {
    recordsToPrint.resize(35);
    for(int i = 0; i < 35; i++) recordsToPrint[i] = CRecords::recordSet(compareFunc);
}
CCalendarMonth::~CCalendarMonth() = default;

struct tm CCalendarMonth::getSelectedRecords(const CRecords & all_records) {
    tm user_date = getYearMonthFromUser();
    tm end_date = user_date;
    end_date.tm_mday += 35;
    findRecords(user_date, end_date, all_records);

    return user_date;
}

void CCalendarMonth::printCalendar(tm & user_date) const {
    string to_show;
    const int FRAME_LENGTH = 225;
    const int GAPS = 12;

    cout << "+" << right << setfill('-')<< setw( FRAME_LENGTH) << '+'<< endl;
    to_show = getMonth(user_date.tm_mon) + " " + to_string(user_date.tm_year + 1900);
    cout << "| " << left << setfill(' ') << setw( FRAME_LENGTH - 2) << to_show << '|' << endl;
    cout << "+" << right << setfill('-')<< setw( FRAME_LENGTH) << '+'<< endl;

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

    for(int week = 0; week < 5; week++){
        tm date1 = user_date;
        date1.tm_mday += 6;
        timegm(&date1);
        to_show = getMonth(user_date.tm_mon) + " " + to_string(user_date.tm_mday) + " " + to_string(user_date.tm_year + 1900)
                  + " - " + getMonth(date1.tm_mon) + " " + to_string(date1.tm_mday) + " " + to_string(date1.tm_year + 1900);
        cout << "| " << left << setfill(' ') << setw( FRAME_LENGTH - 2) << to_show << '|' << endl;
        cout << "+" << right << setfill('-') << setw( FRAME_LENGTH) << '+'<< endl;

        int count = 0;
        vector<int> pos(35);
        bool done = true;
        tm tmp{};

        while(true) {
            tmp = user_date;
            stringstream ss;
            ss << "| " << flush;
            for (int day = 7 * week; day < 7 + 7 * week; day++) {
                if (next(recordsToPrint[day].begin(), pos[day]) == recordsToPrint[day].end()) {
                    ss << right << setfill(' ') << setw(GAPS * 2 + int(getDay(tmp.tm_wday).size()) + 1) << " | " << flush;
                } else {
                    ss << left << setfill(' ') << setw(GAPS * 2 + int(getDay(tmp.tm_wday).size()) - 2) << (*next(recordsToPrint[day].begin(), pos[day]))->name << " | " << flush;
                    pos[day]++;
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
        user_date.tm_mday += 7;
        cout << "+" << right << setfill('-')<< setw( FRAME_LENGTH) << '+'<< endl;
        to_show = "Total amount of events: " + to_string(count);
        cout << "| " << left << setfill(' ') << setw( FRAME_LENGTH - 2) << to_show << '|' << endl;
        cout << "+" << right << setfill('-')<< setw( FRAME_LENGTH) << '+'<< endl;
    }
}


