#ifndef CASINO_WHEEL_H
#define CASINO_WHEEL_H

#include "betting_strategy.h"
#include "logger.h"
#include "pub_sub.h"
#include <map>
#include <set>
#include <thread>
#include <utility>
#include <vector>

namespace casino {
namespace wheel {

    using t_sub_bet_amt = std::map<pub_sub::Subscriber*, double>;

    class Wheel : public pub_sub::Publisher {
    private:
        static const int ROLL_TIME_MS = 20;
        static const int BET_TIME_MS = 20;
        static const int DISPENSE_TIME_MS = 20;

        static constexpr double INITIAL_WHEEL_MONEY = 10000000; // 1crore

        bool run_loop;

        // utility
        logger::Logger myLogger;
        void _start();
        void _rolling();
        void _betting();
        void _dispensing();
        void _switch_to(pub_sub::Event);

        // event management & broadcast
        std::thread worker_thread;
        std::set<pub_sub::Subscriber*> mySubscribers;

        // state management
        double myMoney;
        int myWinningNumber;
        pub_sub::Event myCurrentState;
        std::map<int, t_sub_bet_amt> myBets;
        std::vector<pub_sub::Event> myEventLog;
        std::set<pub_sub::Subscriber*> players_at_table;

    public:
        static const int MIN_NUM = 1;
        static const int MAX_NUM = 36;
        static const int MAX_PLAYER = 5;

        Wheel();
        ~Wheel();

        // utility
        bool place_bet(pub_sub::Subscriber*, betting_strategy::t_bet);
        bool reset_bet(pub_sub::Subscriber*);
        bool taking_bets();

        // event management & broadcast
        void subscribe(pub_sub::Subscriber*);
        void unsubscribe(pub_sub::Subscriber*);
        void publish(pub_sub::Event);

        // state management
        void start();
        void wait();
        void stop();
    };

} // namespace wheel
} // namespace casino

#endif // CASINO_WHEEL_H
