#ifndef KASPAFIL_CAPPLICATION_H
#define KASPAFIL_CAPPLICATION_H

#import <iostream>
#import <ctime>
#import <string>
#import <vector>

using namespace std;

class CDate{
private:
    int year;
    int month;
    int day;

public:
    CDate(const int & year, const int & month, const int & day);
    ~CDate() = default;
    void printDate();
};

class CRecords {
    class CRecord {
    private:
        string name;
        time_t time;
        CDate date;
        string location;
        vector<string> people;
        int repeats;
        int importance;

    public:
        CRecord(const string & name, const time_t & time, const CDate & date, const string & location,
                const vector<string> & people, const int & repeats );
        ~CRecord() = default;
        void deleteRecord();
        void updateRecord();
    };

public:
    ~CRecords() = default;

    void exportRecords() const;
    void importRecords();
    void showRecords() const;

    CRecord findClosest() const;
    CRecord findRecord(const string & searchLocation, const string & name) const;
    CRecord findRecord(const string & searchLocation) const;
    CRecord findRecord(const CDate & searchDate) const;
    CRecord findRecord(const int & importance) const;
    CRecord findRecordByName(const string & searchName) const;

    vector<CRecord> records;
};

class CCalender{
protected:
    CRecords records;
    string name;
    int total_days;

public:
    CCalender(const CRecords & records,const string & name, const int & days);
    virtual ~CCalender() = default;
    virtual void printCalender() const = 0;
    virtual int getTotalRecords() const = 0;
};

class CCalenderDay : public CCalender{
public:
    CCalenderDay(const CRecords & records) : CCalender(records,"Day", 1){};
    ~CCalenderDay() override = default;

    void printCalender() const override;
    int getTotalRecords() const override;
};

class CCalenderWeek : public CCalender{
public:
    CCalenderWeek(const CRecords & records) : CCalender(records,"Week", 7){};
    ~CCalenderWeek() override = default;

    void printCalender() const override;
    int getTotalRecords() const override;
};

class CCalenderMonth : public CCalender{
    int month;
public:
    CCalenderMonth(const CRecords & records, const int & month) : CCalender(records,"Month", 0){
        this->month = month;
    };
    ~CCalenderMonth() override = default;

    void printCalender() const override;
    int getTotalRecords() const override;
    int getDays();
};

#endif
