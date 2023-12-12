#ifndef CASINO_WHEEL_H
#define CASINO_WHEEL_H

#include "logger.h"
#include "pub_sub.h"
#include <map>
#include <set>
#include <thread>
#include <utility>
#include <vector>

namespace casino {
namespace wheel {

    using t_bets = std::map<pub_sub::Subscriber*, int>;

    class Wheel : public pub_sub::Publisher {
    private:
        // utility
        logger::Logger myLogger;
        void _start();
        void _rolling();
        void _betting();
        void _dispensing();
        void _switch_to(pub_sub::Event);
        std::set<pub_sub::Subscriber*> players_at_table;

        // event management & broadcast
        std::thread worker_thread;
        std::set<pub_sub::Subscriber*> mySubscribers;

        // state management
        pub_sub::EventType myCurrentState;
        std::map<int, t_bets> myBets;
        int myMoney;
        std::vector<pub_sub::EventType> myEventLog;

    public:
        static const int MIN_NUM = 1;
        static const int MAX_NUM = 36;
        static const int MAX_PLAYER = 5;

        Wheel();
        ~Wheel();

        // utility
        bool place_bet(pub_sub::Subscriber*, int, int);
        bool reset_bet(pub_sub::Subscriber*);

        // event management & broadcast
        void subscribe(pub_sub::Subscriber*);
        void unsubscribe(pub_sub::Subscriber*);
        void publish(pub_sub::Event);

        // state management
        void start();
        void wait();
    };

} // namespace wheel
} // namespace casino

#endif // CASINO_WHEEL_H
