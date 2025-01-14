#include "CCalendar.h"

using namespace std;

CCalendar::CCalendar(const std::string &name) {
    this->name = name;
}

CCalendar::~CCalendar() = default;

void CCalendar::performActions(const CRecords & all_records){
    if(all_records.getAllRecords().empty()){
        cout << "There are no available events to show!" << endl;
        return;
    }
    struct tm user_date = this->getSelectedRecords(all_records);

    cout << "Showing by: " << this->name << endl;
    printCalendar(user_date);
}

time_t CCalendar::convertDate(tm time) {
    return timegm(&time);
}

bool CCalendar::checkNextMonth(struct tm date) {
    int previous_date = date.tm_mon;
    date.tm_mday += 7;
    timegm(&date);
    if(date.tm_mon != previous_date) return true;
    return false;
}

bool CCalendar::compareFunc(const shared_ptr<CRecords::CRecord> & a, const shared_ptr<CRecords::CRecord> & b){
    if (a->dateStartTime.tm_hour < b->dateStartTime.tm_hour){
        return true;
    }
    else if (a->dateStartTime.tm_hour == b->dateStartTime.tm_hour &&
             a->dateStartTime.tm_min < b->dateStartTime.tm_min){
        return true;
    }
    else if (a->dateStartTime.tm_hour == b->dateStartTime.tm_hour &&
             a->dateStartTime.tm_min == b->dateStartTime.tm_min && a->dateStartTime.tm_sec < b->dateStartTime.tm_sec){
        return true;
    }
    return false;
}

void CCalendar::findRecords(tm begin_date, tm end_date, const CRecords & all_records) {
    auto temp = new CRecords::CRecord;
    temp->dateStartTime = begin_date;
    timegm(&temp->dateStartTime);
    auto itrRecord = all_records.getAllRecordsNonRepeating().lower_bound(shared_ptr<CRecords::CRecord>(temp));

    for(auto record = itrRecord; (record != all_records.getAllRecordsNonRepeating().end() && convertDate((*record)->dateStartTime) >= timegm(&begin_date)
                                  && convertDate((*record)->dateStartTime) <= timegm(&end_date)); record++){


        int days = abs((int)floor(difftime(convertDate((*record)->dateStartTime), timegm(&begin_date)) / (24 * 60 * 60)));
        recordsToPrint[days].insert(shared_ptr<CRecords::CRecord>(*record));
    }

    tm tmp = begin_date;
    int days = abs((int)floor(difftime(timegm(&end_date), timegm(&begin_date))/(24*60*60)));
    for(const auto & record : all_records.getAllRecordsRepeating()){
        for(int i = 0; i < days; i++) {
            if (record->repeats->checkRepeatInDay(tmp)) {
                recordsToPrint[i].insert(record);
            }
            tmp.tm_mday++;
            timegm(&tmp);
        }
        tmp = begin_date;
    }
}

