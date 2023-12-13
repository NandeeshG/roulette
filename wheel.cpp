#include "wheel.h"
#include "betting_strategy.h"
#include "logger.h"
#include "pub_sub.h"

#include <time.h>

namespace casino {
namespace wheel {

    void Wheel::_rolling()
    {
        time_t timeNow = time(NULL);
        pub_sub::Event ev { timeNow + ROLL_TIME_MS / 1000, pub_sub::ROLLING };
        _switch_to(ev);
        myLogger.info("starting rolling");
        int t = ROLL_TIME_MS;
        std::this_thread::sleep_for(std::chrono::milliseconds(t));
        myWinningNumber = (rand() % (MAX_NUM - MIN_NUM + 1)) + MIN_NUM;
        myLogger.info("stopped rolling");
    }

    void Wheel::_dispensing()
    {
        int t = DISPENSE_TIME_MS;
        time_t timeNow = time(NULL);
        pub_sub::Event ev { timeNow + DISPENSE_TIME_MS / 1000, pub_sub::DISPENSING, "winning-number in integer-data", myWinningNumber, 0 };
        _switch_to(ev);
        myLogger.info("dispensing winnings");
        std::this_thread::sleep_for(std::chrono::milliseconds(t / 2));
        for (auto sub_bet : myBets[myWinningNumber]) {
            double winningAmount = sub_bet.second * 1ll * (MAX_NUM - MIN_NUM);
            pub_sub::Event winEv { 0, pub_sub::MONEY_WON, "winning-data", myWinningNumber, winningAmount };
            sub_bet.first->accept(winEv);
        }
        // TODO: For all other who lost, publish event of all the money lost from table for them. (in +ve value)
        std::this_thread::sleep_for(std::chrono::milliseconds(t / 2));
        myLogger.info("winnings dispensed");
    }

    void Wheel::_betting()
    {
        time_t timeNow = time(NULL);
        pub_sub::Event ev { timeNow + BET_TIME_MS / 1000, pub_sub::BETTING };
        int t = BET_TIME_MS;
        _switch_to(ev);
        myLogger.info("betting is open now");
        std::this_thread::sleep_for(std::chrono::milliseconds(t));
        myLogger.info("betting closed");
    }

    void Wheel::_start()
    {
        while (true) {
            if (!run_loop) {
                myLogger.debug("run_loop false");
                return;
            }
            _betting();
            _rolling();
            _dispensing();
        }
    }

    void Wheel::_switch_to(pub_sub::Event event)
    {
        // TODO: We should add mutex usage here
        myCurrentState = event;
        myEventLog.push_back(event);
        publish(event);
    }

    Wheel::Wheel()
    {
        myLogger = logger::Logger("Wheel");
        run_loop = false;
        srand(time(NULL));
        myMoney = INITIAL_WHEEL_MONEY;
    }

    Wheel::~Wheel()
    {
        if (worker_thread.joinable())
            worker_thread.~thread();
    }

    bool Wheel::place_bet(pub_sub::Subscriber* gambler, betting_strategy::t_bet bet)
    {
        int num = bet.first;
        double amt = bet.second;
        if (players_at_table.find(gambler) == players_at_table.end()) {
            if (players_at_table.size() == MAX_PLAYER) {
                myLogger.error("can't take bets of more than ", MAX_PLAYER, " players at once");
                return false;
            }
        }
        if (myCurrentState.event_type != pub_sub::BETTING) {
            myLogger.warning("can't take bets right now");
            return false;
        }
        if (!(MIN_NUM <= num && num <= MAX_NUM)) {
            myLogger.error("num out of range, " + num);
            return false;
        }
        if (gambler == nullptr) {
            myLogger.error("gambler unknown");
            return false;
        }
        players_at_table.insert(gambler);
        myBets[num][gambler] = amt;
        return true;
    }

    bool Wheel::reset_bet(pub_sub::Subscriber* gambler)
    {
        if (gambler == nullptr) {
            myLogger.error("gambler unknown");
            return false;
        }
        for (int i = MIN_NUM; i <= MAX_NUM; ++i) {
            myBets[i][gambler] = 0;
        }
        players_at_table.erase(gambler);
        return true;
    }

    bool Wheel::taking_bets()
    {
        return myCurrentState.event_type == pub_sub::BETTING;
    }

    void Wheel::subscribe(pub_sub::Subscriber* s)
    {
        mySubscribers.insert(s);
    }

    void Wheel::unsubscribe(pub_sub::Subscriber* s)
    {
        if (mySubscribers.count(s))
            mySubscribers.erase(s);
    }

    void Wheel::publish(pub_sub::Event e)
    {
        for (pub_sub::Subscriber* s : mySubscribers) {
            s->accept(e);
        }
    }

    void Wheel::start()
    {
        myLogger.debug("starting thread");
        run_loop = true;
        worker_thread = std::thread([this] { _start(); });
    }

    void Wheel::stop()
    {
        myLogger.debug("stopping thread");
        run_loop = false;
        worker_thread.join();
    }
} // namespace wheel
} // namespace casino
