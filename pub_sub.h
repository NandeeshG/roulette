#ifndef CASINO_PUB_SUB_H
#define CASINO_PUB_SUB_H

#include <time.h>

namespace casino {
namespace pub_sub {

    enum EventType {
        ROLLING,
        BETTING,
        DISPENSING
    };

    class Event {
    public:
        time_t live_till;
        EventType event_type;
        Event(time_t lt, EventType et)
            : live_till(lt)
            , event_type(et)
        {
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