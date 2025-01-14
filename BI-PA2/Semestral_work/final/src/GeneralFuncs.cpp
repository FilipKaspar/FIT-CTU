#include "GeneralFuncs.h"

#include "CCalendar/CCalendar.h"
#include "CCalendar/CCalendarDay.h"
#include "CCalendar/CCalendarWeek.h"
#include "CCalendar/CCalendarMonth.h"

using namespace std;

bool performAction(const char & user_action, CRecords & records){
    if(user_action == 'D' || user_action == 'W' || user_action == 'M'){
        shared_ptr<CCalendar> calender;
        switch(user_action){
            case 'D':{
                calender = make_shared<CCalendarDay>();
                break;
            }
            case 'W':{
                calender = make_shared<CCalendarWeek>();
                break;
            }
            case 'M':{
                calender = make_shared<CCalendarMonth>();
                break;
            }
            default:{
                cout << "\nError occurred! Wrong input action!" << endl;
                break;
            }
        }
        calender->performActions(records);
        return false;
    }

    switch(user_action){
        case 'A':{
            auto * record = new CRecords::CRecord(0);

            if(!records.addRecord(*record)) return false;
            cout << "New record added!" << endl;
            break;
        }
        case 'X':{
            cout << "In order to delete a record, first choose which one you want to delete." << endl;
            records.deleteRecord();
            break;
        }
        case 'U':{
            records.updateRecord();
            break;
        }
        case 'S':{
            records.changeDate();
            break;
        }
        case 'F':{
            CRecords temp;
            temp.setAllRecords(records.findRecords());

            temp.showRecords();
            break;
        }
        case 'R':{
            records.showRecords();
            break;
        }
        case 'E':{
            records.exportRecords();
            break;
        }
        case 'I':{
            records.importRecords();
            break;
        }
        case 'Q':{
            return true;
        }
        default:{
            cout << "\nError occurred! Wrong input action!" << endl;
            break;
        }
    }
    return false;
}

bool checkLeapYear(const tm & Date){
    if(Date.tm_year % 4000 == 0){
        return false;
    }else if(Date.tm_year % 400 == 0){
        return true;
    }else if(Date.tm_year % 100 == 0){
        return false;
    }else if(Date.tm_year % 4 == 0){
        return true;
    }
    return false;
}

bool checkTime(const tm & Date){
    if(Date.tm_year < 1600 || Date.tm_mon < 1 || Date.tm_mon > 12  || Date.tm_hour < 0 || Date.tm_hour > 23 ||
       Date.tm_min < 0 || Date.tm_min > 59 || Date.tm_sec < 0 || Date.tm_sec > 59 || Date.tm_mday < 1){
        return true;
    }

    bool leap = checkLeapYear(Date);

    if(Date.tm_mon == 2){
        if(leap){
            if (Date.tm_mday > 29) return true;
        }else{
            if (Date.tm_mday > 28) return true;
        }
    }else{
        if(Date.tm_mon < 8){
            if(Date.tm_mon % 2 == 0){
                if (Date.tm_mday > 30) return true;
            }else{
                if (Date.tm_mday > 31) return true;
            }

        }else{
            if(Date.tm_mon % 2 == 0){
                if (Date.tm_mday > 31) return true;
            }else{
                if (Date.tm_mday > 28) return true;
            }
        }
    }

    return false;
}

string getMonth(const int & month){
    string months[12] = {"January", "February", "March", "April", "May", "June", "July",
                         "August", "September", "October", "November", "December"};

    return months[month];
}

string getDay(const int & day){
    string days[7] = { "Sunday", "Monday", "Tuesday", "Wednesday" , "Thursday", "Friday", "Saturday"};
    return days[day];
}

tm calculateDuration(tm & start, tm & end){
    time_t diff = difftime(timegm(&end), timegm(&start));

    tm duration{};
    duration.tm_mday = diff / 86400;
    diff %= 86400;
    duration.tm_hour = diff / 3600;
    diff %= 3600;
    duration.tm_min= diff / 60;
    diff %= 60;
    duration.tm_sec = diff;

    return duration;
}

bool checkCollisions(CRecords & records, CRecords::CRecord & new_record, CRecords::CRecord & col_record){
    for(const auto & cur_record : records.getAllRecordsNonRepeating()){
        if(!(*cur_record == new_record)) {
            if (typeid(CEventRepeatNone) == typeid(*new_record.repeats)
                && checkOverlap(timegm(&cur_record->dateStartTime), timegm(&cur_record->dateEndTime),
                                timegm(&new_record.dateStartTime), timegm(&new_record.dateEndTime))) {
                col_record = *cur_record;
                return true;
            } else if (new_record.repeats->checkRepeatInDay(cur_record->dateStartTime)
                       && checkOverlap(convertToSeconds(cur_record->dateStartTime),
                                       convertToSeconds(cur_record->dateEndTime),
                                       convertToSeconds(new_record.dateStartTime),
                                       convertToSeconds(new_record.dateEndTime))) {
                col_record = *cur_record;
                return true;
            }
        }
    }

    for(const auto & cur_record : records.getAllRecordsRepeating()){
        if(!(*cur_record == new_record) && cur_record->repeats->checkRepeatInDay(new_record.dateStartTime)
        && checkOverlap(convertToSeconds(cur_record->dateStartTime), convertToSeconds(cur_record->dateEndTime),
                        convertToSeconds(new_record.dateStartTime), convertToSeconds(new_record.dateEndTime))){
            col_record = *cur_record;
            return true;
        }
    }

    return false;
}

size_t convertToSeconds(tm & time){
    size_t t_time = 0;
    t_time += time.tm_hour * 3600;
    t_time += time.tm_min * 60;
    t_time += time.tm_sec;

    return t_time;
}

bool checkOverlap(size_t cur_record_time_start, size_t cur_record_time_end, size_t new_record_time_start, size_t new_record_time_end){
    if((new_record_time_start > cur_record_time_start && new_record_time_start < cur_record_time_end)
       || (new_record_time_end > cur_record_time_start && new_record_time_end < cur_record_time_end)
       || (new_record_time_start <= cur_record_time_start && new_record_time_end >= cur_record_time_end)){
        return true;
    }
    return false;
}