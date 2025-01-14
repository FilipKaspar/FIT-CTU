#include "CRecords.h"

#include <utility>
#include "UserInput.h"
#include "CCsvFile.h"
#include "GeneralFuncs.h"

using namespace std;

bool CRecords::compareFunc(const shared_ptr<CRecord> & a, const shared_ptr<CRecord> & b) {
    if (a->dateStartTime.tm_year < b->dateStartTime.tm_year){
        return true;
    }
    else if (a->dateStartTime.tm_year == b->dateStartTime.tm_year && a->dateStartTime.tm_mon < b->dateStartTime.tm_mon){
        return true;
    }
    else if (a->dateStartTime.tm_year == b->dateStartTime.tm_year && a->dateStartTime.tm_mon == b->dateStartTime.tm_mon &&
             a->dateStartTime.tm_mday < b->dateStartTime.tm_mday){
        return true;
    }
    else if (a->dateStartTime.tm_year == b->dateStartTime.tm_year && a->dateStartTime.tm_mon == b->dateStartTime.tm_mon &&
             a->dateStartTime.tm_mday == b->dateStartTime.tm_mday && a->dateStartTime.tm_hour < b->dateStartTime.tm_hour){
        return true;
    }
    else if (a->dateStartTime.tm_year == b->dateStartTime.tm_year && a->dateStartTime.tm_mon == b->dateStartTime.tm_mon &&
             a->dateStartTime.tm_mday == b->dateStartTime.tm_mday && a->dateStartTime.tm_hour == b->dateStartTime.tm_hour &&
             a->dateStartTime.tm_min < b->dateStartTime.tm_min){
        return true;
    }
    else if (a->dateStartTime.tm_year == b->dateStartTime.tm_year && a->dateStartTime.tm_mon == b->dateStartTime.tm_mon &&
             a->dateStartTime.tm_mday == b->dateStartTime.tm_mday && a->dateStartTime.tm_hour == b->dateStartTime.tm_hour &&
             a->dateStartTime.tm_min == b->dateStartTime.tm_min && a->dateStartTime.tm_sec < b->dateStartTime.tm_sec){
        return true;
    }
    return false;
}

std::string CRecords::CRecord::dateToString(tm in_date) {
    string date = asctime(&in_date);
    date = date.substr(0,date.length()-1);
    return date;
}

CRecords::CRecord::CRecord(const int &useless) {
    this->name = getNameFromUser();
    this->dateStartTime = getStartTimeFromUser();
    this->dateEndTime = getEndTimeFromUser(this->dateStartTime);
    this->duration = calculateDuration(this->dateStartTime, this->dateEndTime);
    this->location = getLocationFromUser();
    this->description = getDescriptionFromUser();
    this->people = getPeopleFromUser();
    this->tags = getTagsFromUser();
    this->repeats = getRepeatsFromUser();
    this->repeats->addDate(this->dateStartTime, this->dateEndTime);
}

bool CRecords::addRecord(CRecord & srcRecord) {
    CRecords::CRecord col_record;

    if(!checkCollision(srcRecord)) return false;

    auto new_record = shared_ptr<CRecord>(&srcRecord);
    records.insert(new_record);

    if(typeid(*srcRecord.repeats) == typeid(CEventRepeatNone)) recordsNonRepeating.insert(new_record);
    else recordsRepeating.insert(new_record);

    return true;
}

void CRecords::CRecord::printRecord() {
    const int RECORD_FRAME_LENGTH = 80;
    string dur = to_string(this->duration.tm_hour) + " hours " + to_string(this->duration.tm_min) + " minutes "
            + to_string(this->duration.tm_sec) + " seconds";

    cout << "          +" << right << setfill('-')<< setw( RECORD_FRAME_LENGTH) << '+'<< endl;
    cout << "          | Event name: " << left << setfill(' ')<< setw( RECORD_FRAME_LENGTH - 14) << this->name << '|'<< endl;
    cout << "          +" << right << setfill('-')<< setw( RECORD_FRAME_LENGTH) << '+'<< endl;
    cout << "          | Event start date: " << left << setfill(' ')<< setw( RECORD_FRAME_LENGTH - 20) << dateToString(this->dateStartTime) << '|'<< endl;
    cout << "          | Event end date: " << left << setfill(' ')<< setw( RECORD_FRAME_LENGTH - 18) << dateToString(this->dateEndTime) << '|'<< endl;
    cout << "          | Event duration: " << left << setfill(' ')<< setw( RECORD_FRAME_LENGTH - 18) << dur << '|'<< endl;
    if(this->location.empty()) cout << "          | Event location: " << left << setfill(' ')<< setw( RECORD_FRAME_LENGTH - 18) << "Not set" << '|'<< endl;
    else cout << "          | Event location: " << left << setfill(' ')<< setw( RECORD_FRAME_LENGTH - 18) << this->location << '|'<< endl;
    cout << "          | Event description: " << left << setfill(' ')<< setw( RECORD_FRAME_LENGTH - 21) << this->description << '|'<< endl;
    cout << "          +" << right << setfill('-')<< setw( RECORD_FRAME_LENGTH) << '+'<< endl;
    cout << "          | Event repeats: " << left << setfill(' ')<< setw( RECORD_FRAME_LENGTH - 17) << this->repeats->getTextRepeat() << '|'<< endl;
    cout << "          | People attending the event: " << right << setfill(' ')<< setw( RECORD_FRAME_LENGTH - 29) << '|'<< endl;
    if (this->people.empty()) cout << "          |     " << left << setfill(' ')<< setw( RECORD_FRAME_LENGTH - 6) << "None" << '|'<< endl;
    else {
        for (const auto &person: this->people) {
            cout << "          |     " << left << setfill(' ') << setw(RECORD_FRAME_LENGTH - 6) << person << '|' << endl;
        }
    }
    cout << "          +" << right << setfill('-')<< setw( RECORD_FRAME_LENGTH) << '+'<< endl;
    cout << "          | Tags: " << right << setfill(' ')<< setw( RECORD_FRAME_LENGTH - 7) << '|'<< endl;
    if (this->tags.empty()) cout << "          |     " << left << setfill(' ')<< setw( RECORD_FRAME_LENGTH - 6) << "None" << '|'<< endl;
    else {
        string all_tags;
        for (const auto &tag: this->tags) {
            all_tags += "#" + tag + " ";
        }
        cout << "          |     " << left << setfill(' ') << setw(RECORD_FRAME_LENGTH - 6) << all_tags << '|' << endl;
    }
    cout << "          +" << right << setfill('-')<< setw( RECORD_FRAME_LENGTH) << '+'<< endl;
}

void CRecords::showRecords() const {
    const int RECORDS_FRAME_LENGTH = 100;

    if(records.empty()){
        cout << "There are no events to show!" << endl;
        return;
    }

    struct tm previousDate{};
    previousDate.tm_year = -5; // just to pass the first iteration of for loop

    for(const auto & record : records){
        if (previousDate.tm_mon != record->dateStartTime.tm_mon || previousDate.tm_year != record->dateStartTime.tm_year){
            cout << "|" << right << setfill('~')<< setw( RECORDS_FRAME_LENGTH) << '|'<< endl;
            cout << " " << getMonth(record->dateStartTime.tm_mon) << " " << record->dateStartTime.tm_year + 1900 << endl;
        }
        record->printRecord();
        previousDate = record->dateStartTime;
    }
}

bool CRecords::deleteRecord() {
    CRecords sel_records;
    shared_ptr<CRecord> foundRecord;

    sel_records.setAllRecords(this->findRecords());
    if(sel_records.records.empty()) return false;

    if(sel_records.records.size() != 1) foundRecord = sel_records.selectFromRecords();
    else foundRecord = *sel_records.records.begin();

    cout << "Selected record: " << endl;
    foundRecord->printRecord();

    cout << "Do you wish to continue?" << endl;
    if(!getUserYesOrNo()) {
        cout << "Record wasn't deleted!" << endl;
        return false;
    }

    auto itrRecord = find(records.begin(), records.end(), foundRecord);

    if (itrRecord != records.end()) {
        records.erase(itrRecord);

        if(typeid(*foundRecord->repeats) == typeid(CEventRepeatNone)){
            auto pos = find(recordsNonRepeating.begin(), recordsNonRepeating.end(),foundRecord);
            if(pos != recordsNonRepeating.end()) recordsNonRepeating.erase(pos);
            else {
                throw invalid_argument("Removing an instance from recordsRepeating failed!");
            }
        }
        else {
            auto pos = find(recordsRepeating.begin(), recordsRepeating.end(),foundRecord);
            if(pos != recordsRepeating.end()) recordsRepeating.erase(pos);
            else {
                throw invalid_argument("Removing an instance from recordsRepeating failed!");
            }
        }
    }
    else{
        throw invalid_argument("Error has occurred in function deleteRecord while deleting recordsToPrint!");
    }

    cout << "Record deleted!" << endl;

    return true;
}


CRecords::recordSet CRecords::findRecordOptions(recordSet &records_to_select, int &user_action) {
    recordSet selected_records = recordSet(compareFunc);

    auto it = findOptions.find(user_action);
    if(it == findOptions.end()) throw invalid_argument("ERROR: Unexpected error occurred!");

    selected_records = it->second(records_to_select);

    if(selected_records.empty()) cout << "No records were found!" << endl;
    return selected_records;
}
CRecords::recordSet CRecords::findRecords() {
    recordSet selected_records = recordSet(compareFunc);

    if(this->records.empty()){
        cout << "There are no records!" << endl;
        return {};
    }

    for(int i = 0; i < 2; i++){
        int user_action = searchFindOptions();
        if(user_action == 0) return selected_records;

        if(i >= 1){
            cout << "Do you want to add requirement (&&) or option (||) to choose more records with?" << endl;
            cout << "Press: \n"
                    "\t0. To quit\n"
                    "\t1. For additional requirement (&&)\n"
                    "\t2. For additional option (||)" << endl;

            int action = getNumber(0,2);

            if(action == 1) selected_records = this->findRecordOptions(selected_records, user_action);
            if(action == 2){
                recordSet temp = recordSet(compareFunc);

                temp = this->findRecordOptions(this->records, user_action);
                for(const auto & record : temp){
                    if(find(selected_records.begin(), selected_records.end(), record) == selected_records.end()) selected_records.emplace(record);
                }
            }
            else return selected_records;
        }
        else{
            selected_records = this->findRecordOptions(this->records, user_action);
        }
    }

    return selected_records;
}

std::shared_ptr<CRecords::CRecord> CRecords::selectFromRecords() const {
    string input;
    int user_input;

    this->showRecords();
    cout << "The record on top is marked \"1\"" << endl;
    user_input = getNumber(1,this->records.size());
    return *next(this->records.begin(), user_input - 1);
}

CRecords::CRecords() {
    records = recordSet(compareFunc);
    recordsRepeating = recordSet(compareFunc);
    recordsNonRepeating = recordSet(compareFunc);
}

void CRecords::exportRecords() {
    const string folder = "examples";
    recordSet selected_records = recordSet(compareFunc);

    if(records.empty()){
        cout << "There are no available records to be exported!" << endl;
        return;
    }

    cout << "Press: "
            "\t0. If you want to exit exporting\n"
            "\t1. If you want to export certain events\n"
            "\t2. If you want to export all events" << endl;

    int action = getNumber(0,2);
    if(action == 0) return;
    if(action == 1) selected_records = findRecords();

    string filename = getFileNameFromUser();
    if(std::filesystem::exists(folder + "/" + filename + ".csv")){
        cout << "File with a same name already exists. Do you want to overwrite that file?" << endl;
        if(!getUserYesOrNo()) return;
    }

    CCsvFile file(folder + "/" + filename + ".csv");
    if(!file.checkFileOpened()) return;

    if(action == 1) file.transferToFile(selected_records);
    if(action == 2) file.transferToFile(records);
    cout << "Events successfully exported!" << endl;
}

void CRecords::importRecords() {
    const string folder = "examples";

    if(!records.empty()){
        cout << "There has been some created events! Importing different ones will result in mixing them up with current ones!" << endl;
        cout << "Do you want to still proceed and import different events?" << endl;
        if(!getUserYesOrNo()) return;
    }

    string filename = getFileNameFromUser();
    if(!filesystem::exists(folder + "/" + filename + ".csv")){
        cout << "File with this name does not exist!" << endl;
        return;
    }

    CCsvFile file(folder + "/" + filename + ".csv", 1);
    if(!file.checkFileOpened()) return;

    try {
        file.transferFromFile(*this);
    }
    catch(...){
        cout << "There was a problem loading events!" << endl;
        return;
    }
}

void CRecords::updateRecord() {
    cout << "In order to update a record, first choose which one you want to update." << endl;
    bool success = this->deleteRecord();
    if(!success) return;

    cout << "Now input new criteria for the event." << endl;
    auto * record = new CRecords::CRecord(0);
    this->addRecord(*record);

    cout << "Event successfully updated!" << endl;
}

void CRecords::changeDate() {
    CRecords sel_records;
    shared_ptr<CRecord> foundRecord;

    cout << "In order to change date of a record, first choose which event you want to change." << endl;
    sel_records.setAllRecords(this->findRecords());
    if(sel_records.records.empty()) return;

    if(sel_records.records.size() != 1) foundRecord = sel_records.selectFromRecords();
    else foundRecord = *sel_records.records.begin();

    cout << "Selected record: " << endl;
    foundRecord->printRecord();

    cout << "Do you wish to continue?" << endl;
    if(!getUserYesOrNo()) {
        cout << "Record wasn't changed!" << endl;
        return;
    }

    cout << "Now you can input a new date for the event." << endl;

    foundRecord->dateStartTime = getStartTimeFromUser();
    foundRecord->dateEndTime = getEndTimeFromUser(foundRecord->dateStartTime);
    foundRecord->duration = calculateDuration(foundRecord->dateStartTime, foundRecord->dateEndTime);

    if(!checkCollision(*foundRecord)) return;

    cout << "Date has been successfully changed!" << endl;
}

const CRecords::recordSet & CRecords::getAllRecords() const{
    return this->records;
}

const CRecords::recordSet & CRecords::getAllRecordsRepeating() const {
    return this->recordsRepeating;
}

const CRecords::recordSet & CRecords::getAllRecordsNonRepeating() const {
    return this->recordsNonRepeating;
}

void CRecords::setAllRecords(CRecords::recordSet in_records){
    this->records = std::move(in_records);
}

CRecords::recordSet CRecords::findRecordByName(recordSet &records_to_select) {
    recordSet selected_records = recordSet(compareFunc);
    auto record = records_to_select.begin();
    string name = getNameFromUser();

    while((record = find_if(record, end(records_to_select),
                            [&name](const shared_ptr<CRecord> & rec) { return (rec->name == name); })) != records_to_select.end()){
        selected_records.emplace(*record);
        record++;
    }

    return selected_records;
}

CRecords::recordSet CRecords::findRecordByLocation(recordSet &records_to_select) {
    recordSet selected_records = recordSet(compareFunc);
    auto record = records_to_select.begin();
    string location = getLocationFromUser();

    while((record = find_if(record, end(records_to_select),
                            [&location](const shared_ptr<CRecord> & rec) { return (rec->location == location); })) != records_to_select.end()){
        selected_records.emplace(*record);
        record++;
    }

    return selected_records;
}

CRecords::recordSet CRecords::findRecordByPerson(CRecords::recordSet &records_to_select) {
    recordSet selected_records = recordSet(compareFunc);
    auto record = records_to_select.begin();
    string name = getPersonFromUser();

    while((record = find_if(record, end(records_to_select),
                            [&name](const shared_ptr<CRecord> & rec)
                            { if(find(rec->people.begin(), rec->people.end(), name)!= rec->people.end()) return true;
                            return false; })) != records_to_select.end() ){
        selected_records.emplace(*record);
        record++;
    }

    return selected_records;
}

CRecords::recordSet CRecords::findRecordByTag(CRecords::recordSet &records_to_select) {
    recordSet selected_records = recordSet(compareFunc);
    auto record = records_to_select.begin();
    string tag = getTagFromUser();

    while((record = find_if(record, end(records_to_select),
                            [&tag](const shared_ptr<CRecord> & rec)
                            { if(find(rec->tags.begin(), rec->tags.end(), tag)!= rec->tags.end()) return true;
                                return false; })) != records_to_select.end() ){
        selected_records.emplace(*record);
        record++;
    }

    return selected_records;
}

CRecords::recordSet CRecords::findRecordByDateBefore(CRecords::recordSet &records_to_select) {
    recordSet selected_records = recordSet(compareFunc);
    tm date = getDateFromUser();
    auto temp = new CRecords::CRecord;
    temp->dateStartTime = date;

    auto itr_record = records_to_select.lower_bound(shared_ptr<CRecords::CRecord>(temp));

    for(auto record = records_to_select.begin(); record != itr_record; record++){
        selected_records.emplace(*record);
    }

    return selected_records;
}

CRecords::recordSet CRecords::findRecordByDateAfter(CRecords::recordSet &records_to_select) {
    recordSet selected_records = recordSet(compareFunc);
    tm date = getDateFromUser();
    auto temp = new CRecords::CRecord;
    temp->dateStartTime = date;

    auto itr_record = records_to_select.lower_bound(shared_ptr<CRecords::CRecord>(temp));

    for(auto record = itr_record; record != records_to_select.end(); record++){
        selected_records.emplace(*record);
    }

    return selected_records;
}

CRecords::recordSet CRecords::findClosestRecord(recordSet &records_to_select) {
    recordSet selected_records = recordSet(compareFunc);
    selected_records.emplace(*records_to_select.begin());

    return selected_records;
}

bool CRecords::checkCollision(CRecords::CRecord & new_record){
    CRecords::CRecord col_record;

    while(checkCollisions(*this, new_record, col_record)){
        cout << "There is already a different event at the same time!" << endl;
        cout << "Event at the same time:" << endl;
        col_record.printRecord();

        cout << "Do you want to know the next available time for your new record?" << endl;
        if(getUserYesOrNo()) {
            CRecords::CRecord next_record = col_record;

            while(checkCollisions(*this, next_record, next_record)){
                next_record.dateStartTime = next_record.dateEndTime;
                next_record.dateStartTime.tm_sec++;
                next_record.dateEndTime = next_record.dateStartTime;
                timegm(&next_record.dateStartTime);
            }

            next_record.dateStartTime.tm_year = new_record.dateStartTime.tm_year;
            next_record.dateStartTime.tm_mon = new_record.dateStartTime.tm_mon;
            next_record.dateStartTime.tm_mday = new_record.dateStartTime.tm_mday;

            cout << "Next available time is at: " << CRecord::dateToString(next_record.dateStartTime) << endl;
        }

        cout << "Do you wish to change the date and time for your new event?" << endl;
        if(getUserYesOrNo()){
            new_record.dateStartTime = getStartTimeFromUser();
            new_record.dateEndTime = getEndTimeFromUser(new_record.dateStartTime);
            new_record.duration = calculateDuration(new_record.dateStartTime, new_record.dateEndTime);
        }
        else{
            cout << "Your new event WASN'T added!" << endl;
            return false;
        }
    }

    return true;
}

const unordered_map<int, function<CRecords::recordSet(CRecords::recordSet & records_to_select)>> CRecords::findOptions = {
        {1, [](CRecords::recordSet & records_to_select) {
            return CRecords::findRecordByDateBefore(records_to_select); } },
        {2, [](CRecords::recordSet & records_to_select) {
            return CRecords::findRecordByDateAfter(records_to_select); } },
        {3, [](CRecords::recordSet & records_to_select) {
            return CRecords::findRecordByName(records_to_select); } },
        {4, [](CRecords::recordSet & records_to_select) {
            return CRecords::findRecordByLocation(records_to_select); } },
        {5, [](CRecords::recordSet & records_to_select) {
            return CRecords::findRecordByPerson(records_to_select); } },
        {6, [](CRecords::recordSet & records_to_select) {
            return CRecords::findRecordByTag(records_to_select); } },
        {7, [](CRecords::recordSet & records_to_select) {
            return CRecords::findClosestRecord(records_to_select); } },
};

bool operator==(const CRecords::CRecord & a, const CRecords::CRecord & b) {
    if(&a == &b){
        return true;
    }
    return false;
}
