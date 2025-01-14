#include "CRecords.h"
#include "UserInput.h"
#include "GeneralFuncs.h"

using namespace std;

void startCalender(){
    char user_action;
    CRecords records;
    bool exit = false;

    while(!exit) {
        try {
            user_action = getUserAction();
            exit = performAction(user_action, records);
        }
        catch(const invalid_argument & e){
            cout << e.what() << endl;
            return;
        }

        sleep(2);
    }
}

int main() {
    cout << "📅 Welcome to SuprDuprKalendaros 2.0!\n" << endl;

    startCalender();

    cout << "I already miss u 💜🐊." << endl;
    return 0;
}
