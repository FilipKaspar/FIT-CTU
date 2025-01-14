#include "CRecords.h"
#include "UserInput.h"
#include "GeneralFuncs.h"
#include "CEventRepeat/CEventRepeat.h"

using namespace std;

std::string getStringFromUser(const unsigned long int & max, const unsigned long int & min){
    string input;

    cout << R"(Do not include "," symbol!)" << endl;
    while(true){
        if (getline(cin , input) && input.length() <= max && input.length() >= min
            && strchr(input.c_str(), ',') == nullptr){
            return input;
        }

        checkEOF();

        cout << "The minimum length is " << min << " and maximum length is " <<
                max << " characters and no \",\" is allowed!" << endl;
    }
}

string getNameFromUser(){
    string input;
    const unsigned long int MAX_NAME_LENGTH = 28;
    const unsigned long int MIN_NAME_LENGTH = 3;

    cout << "Input name of the Event:" << endl;
    input = getStringFromUser(MAX_NAME_LENGTH, MIN_NAME_LENGTH);

    return input;
}

struct tm getDateTimeFromUser(){
    struct tm date{};
    string input;

    bool repeat = false;
    bool err;

    do{
        err = false;
        if (repeat) cout << "Wrongly inputted date!" << endl;

        getline(cin , input);
        checkEOF();

        repeat = true;
        try {
            if (sscanf(input.c_str(), "%4d-%2d-%2d%2d:%2d:%2d", &date.tm_year, &date.tm_mon, &date.tm_mday,
                       &date.tm_hour, &date.tm_min, &date.tm_sec) != 6) err = true;
        }
        catch(...){
            cout << "An unexpected error occurred while loading the date and time!" << endl;
            err = true;
        }

        if (checkTime(date)) err = true;
    }while(err);

    date.tm_year -= 1900;
    date.tm_mon -= 1;

    timegm(&date);
    return date;
}

struct tm getStartTimeFromUser(){
    cout << "Input date and time of the start of the Event in \"YYYY-MM-DD HH:MM:SS\" format:" << endl;

    return getDateTimeFromUser();
}

struct tm getEndTimeFromUser(tm startDate){
    cout << "Input time when the event should end. The event is gonna end in the same day it started, if you wish to "
            "make an event last for longer days, make more events!" << endl;
    tm date{};
    tm time{};

    while(true){
        time = getTimeFromUser();
        date = joinDateAndTime(startDate, time);

        if(timegm(&date) >= timegm(&startDate)) break;

        cout << "The end time is before the start time! Try again." << endl;
    }

    return date;
}

tm joinDateAndTime(tm & date, tm & time){
    tm new_date{};

    timegm(&date);
    new_date = date;

    new_date.tm_hour = time.tm_hour;
    new_date.tm_min = time.tm_min;
    new_date.tm_sec = time.tm_sec;

    timegm(&new_date);

    return new_date;
}

struct tm getTimeFromUser(){
    struct tm date{};
    string input;

    bool repeat = false;
    bool err;

    cout << "Input a time in \"HH:MM:SS\" format:" << endl;
    do{
        err = false;
        if (repeat) cout << "Wrongly inputted time!" << endl;

        getline(cin , input);
        checkEOF();

        repeat = true;
        try {
            if (sscanf(input.c_str(), "%2d:%2d:%2d", &date.tm_hour, &date.tm_min, &date.tm_sec) != 3
            || date.tm_hour > 23 || date.tm_min > 59 || date.tm_sec > 59) err = true;
        }
        catch(...){
            cout << "An unexpected error occurred while loading the date and time!" << endl;
            err = true;
        }

    }while(err);

    return date;
}

struct tm getDateFromUser(){
    struct tm Date{};
    string input;

    bool repeat = false;
    bool err;

    cout << "Input a date in \"YYYY-MM-DD\" format:" << endl;
    do{
        err = false;
        if (repeat) cout << "Wrongly inputted date!" << endl;

        getline(cin , input);
        checkEOF();

        repeat = true;
        try {
            if (sscanf(input.c_str(), "%4d-%2d-%2d", &Date.tm_year, &Date.tm_mon, &Date.tm_mday) != 3) err = true;
        }
        catch(...){
            cout << "An unexpected error occurred while loading the date and time!" << endl;
            err = true;
        }

        if (checkTime(Date)) err = true;
    }while(err);

    Date.tm_year -= 1900;
    Date.tm_mon -= 1;

    timegm(&Date);
    return Date;
}

struct tm getYearMonthFromUser(){
    struct tm Date{};
    string input;

    bool repeat = false;
    bool err;

    cout << "Input a date in \"YYYY-MM\" format:" << endl;
    do{
        err = false;
        if (repeat) cout << "Wrongly inputted date!" << endl;

        getline(cin , input);
        checkEOF();

        repeat = true;
        try {
            if (sscanf(input.c_str(), "%4d-%2d", &Date.tm_year, &Date.tm_mon) != 2) err = true;
        }
        catch(...){
            cout << "An unexpected error occurred while loading the date and time!" << endl;
            err = true;
        }

        if (Date.tm_year < 1600 || Date.tm_mon < 1 || Date.tm_mon > 12) err = true;
    }while(err);

    Date.tm_year -= 1900;
    Date.tm_mon -= 1;
    Date.tm_mday = 1;

    timegm(&Date);
    return Date;
}

string getLocationFromUser(){
    string input;
    const unsigned long int MAX_LOCATION_LENGTH = 30;
    const unsigned long int MIN_LOCATION_LENGTH = 3;


    cout << "Input location of the Event:" << endl;
    input = getStringFromUser(MAX_LOCATION_LENGTH, MIN_LOCATION_LENGTH);

    return input;
}

vector<string> getPeopleFromUser(){
    vector<string> input{};
    string temp;
    const int MAX_PEOPLE_LENGTH = 40;
    const int MIN_PEOPLE_LENGTH = 2;
    const int MAX_PEOPLE = 4;


    cout << "Input the names of people at the event or type \"s\" for blank or when you are done inputting (Maximum " << MAX_PEOPLE << " people): " << endl;
    cout << R"(Do not include "," symbol!)" << endl;
    for(int cnt = 0; cnt < MAX_PEOPLE; cnt++){
        getline(cin , temp);
        checkEOF();

        if (temp == "s") return input;
        if (temp.size() > MAX_PEOPLE_LENGTH || temp.size() < MIN_PEOPLE_LENGTH || strchr(temp.c_str(), ',') != nullptr){
            cout << "The longest name allowed is " << MAX_PEOPLE_LENGTH << " and shortest " << MIN_PEOPLE_LENGTH << "!!" << endl;
            cnt--;
            continue;
        }

        input.push_back(temp);
    }

    return input;
}

std::string getPersonFromUser(){
    string temp;
    const int MAX_PEOPLE_LENGTH = 40;
    const int MIN_PEOPLE_LENGTH = 2;

    cout << "Input name of a the person." << endl;
    cout << R"(Do not include "," symbol!)" << endl;

    while(true) {
        getline(cin, temp);
        checkEOF();
        if (temp.size() > MAX_PEOPLE_LENGTH || temp.size() < MIN_PEOPLE_LENGTH ||
            strchr(temp.c_str(), ',') != nullptr) {
            cout << "The longest name allowed is " << MAX_PEOPLE_LENGTH << " and shortest " << MIN_PEOPLE_LENGTH << "!!"
                 << endl;
        }
        else return temp;
    }
}

vector<string> getTagsFromUser(){
    vector<string> input{};
    string temp;
    const int MAX_TAGS_LENGTH = 10;
    const int MIN_TAGS_LENGTH = 2;
    const int MAX_TAGS = 6;


    cout << "Input the tags for the event or type \"s\" for blank or when you are done inputting (Maximum " << MAX_TAGS << " tags): " << endl;
    cout << R"(Do not include "," symbol!)" << endl;
    for(int cnt = 0; cnt < MAX_TAGS; cnt++){
        getline(cin , temp);
        checkEOF();

        if (temp == "s") return input;
        if (temp.size() > MAX_TAGS_LENGTH || temp.size() < MIN_TAGS_LENGTH || strchr(temp.c_str(), ',') != nullptr){
            cout << "The longest name allowed is " << MAX_TAGS_LENGTH << " and shortest " << MIN_TAGS_LENGTH << "!!" << endl;
            cnt--;
            continue;
        }

        input.push_back(temp);
    }

    return input;
}

std::string getTagFromUser(){
    string temp;
    const int MAX_TAGS_LENGTH = 10;
    const int MIN_TAGS_LENGTH = 2;

    cout << "Input a name tag." << endl;
    cout << R"(Do not include "," symbol!)" << endl;

    while(true) {
        getline(cin, temp);
        checkEOF();
        if (temp.size() > MAX_TAGS_LENGTH || temp.size() < MIN_TAGS_LENGTH ||
            strchr(temp.c_str(), ',') != nullptr) {
            cout << "The longest name allowed is " << MAX_TAGS_LENGTH << " and shortest " << MIN_TAGS_LENGTH << "!!"
                 << endl;
        }
        else return temp;
    }
}

std::shared_ptr<CEventRepeat> getRepeatsFromUser(){
    string input;

    cout << "Input how you want your event to be repeating:"
            "\n\"0\" no repeating"
            "\n\"1\" every day"
            "\n\"2\" every week"
            "\n\"3\" every month"
            "\n\"4\" every 14 days" << endl;

    while(true){
        if (getline(cin, input) && input.size() == 1 && isdigit(input[0])){
            return CEventRepeat::repeats.find(stoi(input))->second();
        }
        checkEOF();

        cout << "Input a number in range!!" << endl;
    }

}

string getDescriptionFromUser() {
    string input;
    const unsigned long int MAX_DESCRIPTION_LENGTH = 40;
    const unsigned long int MIN_DESCRIPTION_LENGTH = 3;

    cout << "Input the description for the event." << endl;
    input = getStringFromUser(MAX_DESCRIPTION_LENGTH, MIN_DESCRIPTION_LENGTH);

    return input;
}

void actionOptions(){
    cout << "Which action do you want to perform?\n" << endl;
    cout << "\tC - show calender\n\n"

            "\tA - add event\n"
            "\tX - delete event\n"
            "\tU - update event\n"
            "\tS - change date of the event\n"
            "\tF - find and show specific record\n"
            "\tR - show all events\n\n"

            "\tE - export events\n"
            "\tI - import all events\n\n"

            "\tQ - exit the program\n";
}

void calenderDisplayOptions(){
    cout << "\nIn which format you want to have the calender displayed?\n";
    cout << "\tD - day\n"
            "\tW - week\n"
            "\tM - month\n";
}

void actionDisplayCalender(char & action){
    string temp;
    calenderDisplayOptions();

    cout << "Type your input:" << endl;
    while(true){

        if (getline(cin, temp) && temp.size() == 1 && isalpha(temp[0], locale())){
            action = char(toupper(temp[0]));
            if(action == 'D' || action == 'W' || action == 'M') break;
        }
        checkEOF();

        cout << "Wrong input, allowed are only the letters above!" << endl;
    }
}

void actionInput(char & action){
    string temp;

    cout << "Type your input:" << endl;
    while(true){

        if (getline(cin, temp) && temp.size() == 1 && isalpha(temp[0], locale())){
            action = char(toupper(temp[0]));
            if(action == 'C' || action == 'A' || action == 'X' ||
               action == 'R' || action == 'E' || action == 'I' ||
               action == 'Q' || action == 'F' || action == 'U' ||
               action == 'S') break;
        }
        checkEOF();

        cout << "Wrong input, allowed are only the letters above!" << endl;
    }

    if(action == 'C'){
        actionDisplayCalender(action);
    }
}

char getUserAction(){
    char action = 0;

    actionOptions();
    actionInput(action);

    cout << "\nSelected action: " << action << endl;
    return action;
}

string getFileNameFromUser(){
    string input;
    const unsigned long int MAX_FILENAME_LENGTH = 50;
    const unsigned long int MIN_FILENAME_LENGTH = 1;

    cout << "Input a name of file." << endl;
    input = getStringFromUser(MAX_FILENAME_LENGTH, MIN_FILENAME_LENGTH);

    return input;
}

int getNumber(const int & min, const int & max){
    string input;

    cout << "Select action by typing a number between " << min << " (including) and " << max << " (including)." << endl;
    while(true){
        if (getline(cin , input) && input.length() == 1 && all_of(input.begin(), input.end(), ::isdigit)
           && stoi(input) >= min && stoi(input) <= max){
            return stoi(input);
        }
        checkEOF();

        cout << "The minimum number is " << min << " and maximum is "<< max << "!" << endl;
    }
}

bool getUserYesOrNo(){
    char action;
    string temp;

    cout << "Type yes(Y) or no(N):" << endl;
    while(true){
        if (getline(cin, temp) && temp.size() == 1 && isalpha(temp[0], locale())){
            action = char(toupper(temp[0]));
            if(action == 'Y') return true;
            if(action == 'N') return false;
        }
        checkEOF();

        cout << R"(Wrong input, allowed is only 'y' or 'n'!)" << endl;
    }
}

void checkEOF(){
    if(cin.eof()){
        throw invalid_argument("EOF Registered!");
    }
}

int searchFindOptions(){
    cout << "You can find the record in 7 different ways:\n"
            "\t1. By inputting a date to show previous records\n"
            "\t2. By inputting a date to show upcoming records\n"
            "\t3. By inputting a name\n"
            "\t4. By inputting a location\n"
            "\t5. By inputting a person\n"
            "\t6. By inputting a tag\n"
            "\t7. Or inputting nothing and the closest record will be shown" << endl;

    cout << "Type corresponding number to search for the record or type \"0\" to quit finding." << endl;

    return getNumber(0,7);
}

