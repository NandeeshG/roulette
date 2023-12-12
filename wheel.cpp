#include "wheel.h"
#include "logger.h"
#include "pub_sub.h"

#include <time.h>

namespace casino {
namespace wheel {

    void Wheel::_rolling()
    {
        time_t timeNow = time(NULL);
        pub_sub::Event ev { timeNow + 8, pub_sub::ROLLING, 0 };
        _switch_to(ev);
        myLogger.info("starting rolling");
        std::this_thread::sleep_for(std::chrono::milliseconds(8000));
        myLogger.info("stopped rolling");
    }

    void Wheel::_dispensing()
    {
        time_t timeNow = time(NULL);
        pub_sub::Event ev { timeNow + 7, pub_sub::DISPENSING, 0 };
        _switch_to(ev);
        myLogger.info("dispensing winnings");
        std::this_thread::sleep_for(std::chrono::milliseconds(7000));
        myLogger.info("winnings dispensed");
    }

    void Wheel::_betting()
    {
        time_t timeNow = time(NULL);
        pub_sub::Event ev { timeNow + 15, pub_sub::BETTING, 0 };
        _switch_to(ev);
        myLogger.info("betting is open now");
        std::this_thread::sleep_for(std::chrono::milliseconds(15000));
        myLogger.info("betting closed`");
    }

    void Wheel::_start()
    {
        while (true) {
            _betting();
            _rolling();
            _dispensing();
        }
        // int et = rand() % 3;
        // pub_sub::Event e = pub_sub::Event(0, pub_sub::EventType(et), 0);
        // myLogger.warning("publishing event type: ", e.event_type);
        // publish(e);
    }

    void Wheel::_switch_to(pub_sub::Event event)
    {
        // TODO: We should add mutex usage here
        myCurrentState = event.event_type;
        publish(event);
    }

    Wheel::Wheel()
    {
        myLogger = logger::Logger("Wheel");
    }

    Wheel::~Wheel()
    {
        worker_thread.~thread();
    }

    bool Wheel::place_bet(pub_sub::Subscriber* gambler, int num, int amt)
    {
        if (players_at_table.find(gambler) == players_at_table.end()) {
            if (players_at_table.size() == MAX_PLAYER) {
                myLogger.error("can't take bets of more than 5 players at once");
                return false;
            }
        }
        if (myCurrentState != pub_sub::BETTING) {
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
        worker_thread = std::thread([this] { _start(); });
    }

    void Wheel::wait()
    {
        worker_thread.join();
    }
} // namespace wheel
} // namespace casino
