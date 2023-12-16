#ifndef CASINO_PUB_SUB_H
#define CASINO_PUB_SUB_H

#include <sstream>
#include <string>
#include <time.h>
#include <vector>

namespace casino {
namespace pub_sub {
    /**
     * Betting type signifying first the number on the wheel,
     * and then the amount bet.
     */
    typedef std::pair<int, double> t_bet;
    typedef std::vector<t_bet> t_bets;

    enum EventType {
        UNKNOWN,
        WHEEL_ROLLING,
        BETTING_ON,
        DISTR_WINNINGS,
        PLYR_WON,
        PLYR_LOST,
        BET_ACCEPTED,
        BET_REJECTED
    };

    class Event {
    public:
        time_t generated_on;
        time_t live_till;
        EventType event_type;
        std::string desc;
        std::vector<int> int_data;
        std::vector<double> double_data;
        Event()
            : live_till(INT32_MAX)
            , event_type(UNKNOWN)
            , desc("")
            , int_data(0)
            , double_data(0)
        {
            generated_on = time(NULL);
        }
        Event(time_t lt, EventType et)
            : live_till(lt)
            , event_type(et)
            , desc("")
            , int_data(0)
            , double_data(0)
        {
            generated_on = time(NULL);
        }
        Event(time_t lt, EventType et, std::string de, int id, double dd)
            : live_till(lt)
            , event_type(et)
            , desc(de)
            , int_data(id)
            , double_data(dd)
        {
            generated_on = time(NULL);
        }
        Event(time_t lt, EventType et, std::string de, std::vector<int> id, std::vector<double> dd)
            : live_till(lt)
            , event_type(et)
            , desc(de)
            , int_data(id)
            , double_data(dd)
        {
            generated_on = time(NULL);
        }

        std::string to_string() const
        {
            char timeStr[100];
            strftime(timeStr, sizeof(timeStr), "%Y%m%d:%T:%Z", localtime(&live_till));
            std::stringstream ss;
            ss << "EventType: " << event_type
               << "; LiveTill: " << std::string(timeStr)
               << "; Description: " << desc;

            ss << "; Double-Data: (";
            for (double d : double_data) {
                ss << d << ',';
            }
            ss << "); Integer-Data: ";
            for (int i : int_data) {
                ss << i << ',';
            }
            ss << ")";
            return ss.str();
        }
    };

    class Subscriber {
    public:
        virtual void accept(Event) = 0;
    };

    class Publisher {
    public:
        virtual void subscribe(Subscriber*) = 0;
        virtual void unsubscribe(Subscriber*) = 0;
    };

} // namespace pub_sub
} // namespace casino

#endif // CASINO_PUB_SUB_H
