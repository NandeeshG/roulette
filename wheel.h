#ifndef CASINO_WHEEL_H
#define CASINO_WHEEL_H

#include "logger.h"
#include "pub_sub.h"
#include <set>
#include <thread>

namespace casino {
namespace wheel {

    class Wheel : public pub_sub::Publisher {
    private:
        std::set<pub_sub::Subscriber*> mySubscribers;
        std::thread worker_thread;
        logger::Logger myLogger;
        void _start();

    public:
        Wheel();
        void subscribe(pub_sub::Subscriber* s);
        void unsubscribe(pub_sub::Subscriber* s);
        void publish(pub_sub::Event e);
        void start();
        void wait();
    };

} // namespace wheel
} // namespace casino

#endif // CASINO_WHEEL_H