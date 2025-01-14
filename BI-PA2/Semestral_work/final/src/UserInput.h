#ifndef MAIN_CPP_USERINPUT_H
#define MAIN_CPP_USERINPUT_H

#include <vector>
#include <string>
#include <cstring>

/**
 * Basic function for getting action from a user.
 * @return Return char that represents user selected action.
 */
char getUserAction();
/**
 * Get action selected by user from the menu.
 * @param action Char where user input is going to be saved.
 */
void actionInput(char & action);
/**
 * If user has chosen calendar view, he will be asked to choose from
 * day, week or month calendar view.
 * @param action Char where user input is going to be saved.
 */
void actionDisplayCalender(char & action);
/**
 *
 * @param date
 * @param time
 * @return
 */
tm joinDateAndTime(tm & date, tm & time);

/**
 * Get name from the user.
 * @return By user inputted name.
 */
std::string getNameFromUser();
/**
 * Get date in "YYYY:MM:DD" format from the user.
 * @return By user inputted date.
 */
struct tm getDateFromUser();
/**
 * Get date in "HH:MM:SS" format from the user.
 * @return By user inputted date.
 */
struct tm getTimeFromUser();
/**
 * Get start date in "YYYY:MM:DD HH:MM:SS" format from the user.
 * @return By user inputted date.
 */
struct tm getStartTimeFromUser();
/**
 * Get date in "YYYY:MM:DD HH:MM:SS" format from the user.
 * @return By user inputted date.
 */
struct tm getDateTimeFromUser();
/**
 * Get end date in "YYYY:MM:DD HH:MM:SS" format from the user.
 * @return By user inputted date.
 */
struct tm getEndTimeFromUser(tm startDate);
/**
 * Get date in "YYYY:MM" format from the user.
 * @return By user inputted date.
 */
struct tm getYearMonthFromUser();
/**
 * Get location from the user.
 * @return By user inputted location.
 */
std::string getLocationFromUser();/**
 * Get description from the user.
 * @return By user inputted description.
 */
std::string getDescriptionFromUser();
/**
 * Get People attending the event from the user.
 * @return By user inputted people.
 */
std::vector<std::string> getPeopleFromUser();
/**
 * Get person for the event from the user.
 * @return By user inputted person.
 */
std::string getPersonFromUser();
/**
 * Get tags for the event from the user.
 * @return By user inputted people.
 */
std::vector<std::string> getTagsFromUser();
/**
 * Get a tag for a event from user.
 * @return Tag inputted by user.
 */
std::string getTagFromUser();
/**
 * Get how often a event should repeat.
 * @return By user inputted how often event should repeat.
 */
std::shared_ptr<CEventRepeat> getRepeatsFromUser();
/**
 * Get the name of a file from where events will be either saved or read from.
 * @return By user inputted name of a file.
 */
std::string getFileNameFromUser();
/**
 * Get a number from user within specific range, while 1 being always the lowest.
 * @param max Maximal number used in range.
 * @return Number that user chose.
 */
int getNumber(const int & min, const int & max);
/**
 * Takes care of all string user inputs.
 * @param max Maximal length of a string.
 * @param min Minimal length of a string.
 * @return By user inputted string.
 */
std::string getStringFromUser(const unsigned long int & max, const unsigned long int & min);
/**
 * Get "yes" or "no" decision from the user.
 * @return Char representing yes or no chose by the user.
 */
bool getUserYesOrNo();
/**
 * Calendar view options from which user can select.
 */
void calenderDisplayOptions();
/**
 * Basic options from which user can select.
 */
void actionOptions();
/**
 * Checks if EOF has been inputted. If so, program will close.
 */
void checkEOF();
/**
 * Print search options and get user's option.
 * @return Number indicating the search function.
 */
int searchFindOptions();


#endif
