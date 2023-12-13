#ifndef CASINO_PUB_SUB_H
#define CASINO_PUB_SUB_H

#include <sstream>
#include <string>
#include <time.h>

namespace casino {
namespace pub_sub {

    enum EventType {
        UNKNOWN,
        ROLLING,
        BETTING,
        DISPENSING,
        MONEY_WON,
        MONEY_LOST,
        BET_PLACED
    };

    class Event {
    public:
        time_t live_till;
        EventType event_type;
        std::string desc;
        int int_data;
        double double_data;
        Event()
            : live_till(0)
            , event_type(UNKNOWN)
            , desc("")
            , int_data(0)
            , double_data(0)
        {
        }
        Event(time_t lt, EventType et)
            : live_till(lt)
            , event_type(et)
            , desc("")
            , int_data(0)
            , double_data(0)
        {
        }
        Event(time_t lt, EventType et, std::string de, int id, double dd)
            : live_till(lt)
            , event_type(et)
            , desc(de)
            , int_data(id)
            , double_data(dd)
        {
        }
        std::string to_string() const
        {
            char timeStr[100];
            strftime(timeStr, sizeof(timeStr), "%Y%m%d:%T:%Z", localtime(&live_till));
            std::stringstream ss;
            ss << "EventType: " << event_type
               << ", LiveTill: " << std::string(timeStr)
               << ", Description: " << desc
               << ", Double-Data: " << double_data
               << ", Integer-Data: " << int_data;
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
