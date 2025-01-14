#ifndef MAIN_CEVENTREPEAT_H
#define MAIN_CEVENTREPEAT_H

#include <string>
#include <map>
#include <memory>
#include <functional>
#include <set>

class CEventRepeat{
public:
    /**
     * Add date to the class variable.
     * @param start_date Start date of the event.
     * @param end_date End date of the event.
     */
    void addDate(const tm & start_date, const tm & end_date);
    /**
     * Get string form of repetition of an event.
     * @return Sentence describing how often event repeats.
     */
    virtual std::string getTextRepeat() const = 0;
    /**
     * Check's if even_start_date repeats in the inputted date.
     * @param user_date Date that is being compared to the event's date.
     * @return True if user_date repeats in the same day as event_start_date does.
     */
    virtual bool checkRepeatInDay(const tm &user_date) const = 0;
    /**
     * Get corresponding number to how often event repeats.
     * @return How often event repeats.
     */
    virtual int getSaveNumber() const = 0;

    /**
     * Map that assigns each repetition its shared pointer.
     */
    const static std::unordered_map<int, std::function<std::shared_ptr<CEventRepeat>()>> repeats;

protected:
    tm event_start_date{};
    tm event_end_date{};
};




#endif
