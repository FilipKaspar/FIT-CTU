#include "CCsvFile.h"
#include "CEventRepeat/CEventRepeatDaily.h"

using namespace std;

CCsvFile::CCsvFile(const string & file_Name, const char & delimiter) : fs(), delimiter(delimiter){
    fs.open(file_Name, ios::out);
}

CCsvFile::CCsvFile(const string &file_Name, const int & useless, const char & delimiter) : fs(), delimiter(delimiter){
    fs.open(file_Name, ios::in);
}

CCsvFile::~CCsvFile(){
    fs.close();
}

bool CCsvFile::checkFileOpened() {
    if(!this->fs){
        cout << "Couldn't open the file! Possible due to file being opened by user.\nClose the file and try again!" << endl;
        return false;
    }
    return true;
}

void CCsvFile::transferToFile(const CRecords::recordSet & records) {
    for(const auto & itr_record : records) {
        this->fs << itr_record->name << delimiter;

        this->fs << itr_record->dateStartTime.tm_year << delimiter;
        this->fs << itr_record->dateStartTime.tm_mon << delimiter;
        this->fs << itr_record->dateStartTime.tm_mday << delimiter;
        this->fs << itr_record->dateStartTime.tm_hour << delimiter;
        this->fs << itr_record->dateStartTime.tm_min << delimiter;
        this->fs << itr_record->dateStartTime.tm_sec << delimiter;

        this->fs << itr_record->dateEndTime.tm_year << delimiter;
        this->fs << itr_record->dateEndTime.tm_mon << delimiter;
        this->fs << itr_record->dateEndTime.tm_mday << delimiter;
        this->fs << itr_record->dateEndTime.tm_hour << delimiter;
        this->fs << itr_record->dateEndTime.tm_min << delimiter;
        this->fs << itr_record->dateEndTime.tm_sec << delimiter;

        this->fs << itr_record->location << delimiter;
        this->fs << itr_record->description << delimiter;

        this->fs << itr_record->people.size() << delimiter;
        for(const auto & itr_people : itr_record->people){
            this->fs << itr_people << delimiter;
        }

        this->fs << itr_record->tags.size() << delimiter;
        for(const auto & itr_tag : itr_record->tags){
            this->fs << itr_tag << delimiter;
        }

        this->fs << itr_record->repeats->getSaveNumber() << delimiter << endl;
    }
}

void CCsvFile::transferFromFile(CRecords &records) {
    vector<string> lines;
    string cell;

    while(getline(this->fs, cell)){
        lines.emplace_back(cell);
    }

    for(const auto & csv_line : lines) {
        auto * temp = new CRecords::CRecord();
        istringstream s(csv_line);
        vector<string> cells;

        while (getline(s, cell, delimiter)) {
            cells.emplace_back(cell);
        }

        if(cells.size() > 28 || cells.size() < 18){
            cout << "File doesn't have any events! Or missing information in"
                    " one of the events!" << endl;
            delete temp;
            return;
        }

        temp->name = cells[0];

        temp->dateStartTime.tm_year = stoi(cells[1]);
        temp->dateStartTime.tm_mon = stoi(cells[2]);
        temp->dateStartTime.tm_mday = stoi(cells[3]);
        temp->dateStartTime.tm_hour = stoi(cells[4]);
        temp->dateStartTime.tm_min = stoi(cells[5]);
        temp->dateStartTime.tm_sec = stoi(cells[6]);

        temp->dateEndTime.tm_year = stoi(cells[7]);
        temp->dateEndTime.tm_mon = stoi(cells[8]);
        temp->dateEndTime.tm_mday = stoi(cells[9]);
        temp->dateEndTime.tm_hour = stoi(cells[10]);
        temp->dateEndTime.tm_min = stoi(cells[11]);
        temp->dateEndTime.tm_sec = stoi(cells[12]);

        temp->location = cells[13];
        temp->description = cells[14];

        int i = 0;
        for(; i < stoi(cells[15]); i++){
            temp->people.emplace_back(cells[16+i]);
        }

        int k = 0;
        for(; k < stoi(cells[16+i]); k++){
            temp->tags.emplace_back(cells[17+i+k]);
        }

        int num = stoi(cells[17+i+k]);
        temp->repeats = CEventRepeat::repeats.find(num)->second();

        timegm(&temp->dateStartTime);

        timegm(&temp->dateEndTime);

        temp->duration = calculateDuration(temp->dateStartTime, temp->dateEndTime);
        temp->repeats->addDate(temp->dateStartTime, temp->dateEndTime);

        records.addRecord(*temp);
    }
    cout << "Events successfully imported!" << endl;
}

