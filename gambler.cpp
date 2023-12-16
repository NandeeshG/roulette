#include "gambler.h"
#include "betting_strategy.h"
#include "logger.h"
#include "pub_sub.h"
#include "wheel.h"

namespace casino {
namespace gambler {

    int Gambler::GAMBLER_COUNT = 0;

    Gambler::Gambler(betting_strategy::Strategy* strategy, wheel::Wheel* wheel)
        : myGamblerID(GAMBLER_COUNT++)
        , myStrategy(strategy)
    {
        do_auto_betting = false;
        myWheel = wheel;
        myLogger = logger::Logger("Gambler<" + std::to_string(myGamblerID) + ">");
        myMoney = myStrategy->get_stats().get_starting_money();
        if (myMoney <= 0) {
            myLogger.severe("starting money can't be <= 0, we were given: ", myMoney);
        }
    }

    void Gambler::start_auto_betting()
    {
        do_auto_betting = true;
    }

    void Gambler::stop_auto_betting()
    {
        do_auto_betting = false;
    }

    void Gambler::start_subscription()
    {
        myWheel->subscribe(this);
    }

    void Gambler::stop_subscription()
    {
        myWheel->unsubscribe(this);
    }

    void Gambler::accept(pub_sub::Event e)
    {
        myStrategy->accept(e);
        if (e.event_type == pub_sub::PLYR_WON) {
            myLogger.debug(log_prefix() + "received event: ", e.to_string());
            account_win(e);
        } else if (do_auto_betting && e.event_type == pub_sub::BETTING_ON) {
            auto_place_bet();
        } else {
            myLogger.debug(log_prefix() + "received event: ", e.to_string());
        }
    }

    void Gambler::account_win(pub_sub::Event e)
    {
        myStrategy->get_stats().print();
        myMoney += e.double_data;
        myLogger.warning(log_prefix("has won: " + std::to_string(e.double_data) + " with total money " + std::to_string(myMoney)));
    }

    void Gambler::auto_place_bet()
    {
        betting_strategy::t_bet bet = myStrategy->get_next_bet();
        place_bet(bet);
    }

    void Gambler::place_bet(betting_strategy::t_bet bet)
    {
        double amt = bet.second;
        if (amt > myMoney) {
            myStrategy->get_stats().print();
            myLogger.severe(log_prefix("can't place trades with leverage as of now."));
        }
        bool ret = myWheel->place_bet(this, bet);
        if (!ret) {
            myLogger.error(log_prefix("failed placing bet."));
            return;
        } else {
            myLogger.warning(log_prefix(), "placing bet on ", bet.first, " with ", bet.second, "rs");
            myStrategy->accept(pub_sub::Event { 0, pub_sub::BET_ACCEPTED, "", bet.first, bet.second });
        }
        myMoney -= amt;
    }

    std::string Gambler::log_prefix()
    {
        return "GamblerID: " + std::to_string(myGamblerID) + " ";
    }

    std::string Gambler::log_prefix(std::string suffix)
    {
        return "GamblerID: " + std::to_string(myGamblerID) + " " + suffix;
    }

} // namespace gambler
} // namespace casino
